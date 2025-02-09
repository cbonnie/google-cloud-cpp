// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "google/cloud/internal/oauth2_service_account_credentials.h"
#include "google/cloud/internal/base64_transforms.h"
#include "google/cloud/internal/filesystem.h"
#include "google/cloud/internal/oauth2_credential_constants.h"
#include "google/cloud/internal/openssl_util.h"
#include "google/cloud/internal/random.h"
#include "google/cloud/testing_util/mock_fake_clock.h"
#include "google/cloud/testing_util/mock_http_payload.h"
#include "google/cloud/testing_util/mock_rest_client.h"
#include "google/cloud/testing_util/mock_rest_response.h"
#include "google/cloud/testing_util/status_matchers.h"
#include "absl/strings/str_split.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <chrono>

namespace google {
namespace cloud {
namespace oauth2_internal {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace {

using ::google::cloud::rest_internal::HttpPayload;
using ::google::cloud::testing_util::FakeClock;
using ::google::cloud::testing_util::MockHttpPayload;
using ::google::cloud::testing_util::MockRestClient;
using ::google::cloud::testing_util::MockRestResponse;
using ::google::cloud::testing_util::StatusIs;
using ::testing::_;
using ::testing::A;
using ::testing::ByMove;
using ::testing::Contains;
using ::testing::HasSubstr;
using ::testing::Not;
using ::testing::Return;

constexpr char kAltScopeForTest[] =
    "https://www.googleapis.com/auth/devstorage.full_control";
// This "magic" assertion below was generated from helper script,
// "make_jwt_assertion_for_test_data.py". Note that when our JSON library dumps
// a string representation, the keys are always in alphabetical order; our
// helper script also takes special care to ensure Python dicts are dumped in
// this manner, as dumping the keys in a different order would result in a
// different Base64-encoded string, and thus a different assertion string.
constexpr char kExpectedAssertionParam[] =
    R"""(eyJhbGciOiJSUzI1NiIsImtpZCI6ImExYTExMWFhMTExMWExMWExMWExMWFhMTExYTExMWExYTExMTExMTEiLCJ0eXAiOiJKV1QifQ.eyJhdWQiOiJodHRwczovL29hdXRoMi5nb29nbGVhcGlzLmNvbS90b2tlbiIsImV4cCI6MTUzMDA2MzkyNCwiaWF0IjoxNTMwMDYwMzI0LCJpc3MiOiJmb28tZW1haWxAZm9vLXByb2plY3QuaWFtLmdzZXJ2aWNlYWNjb3VudC5jb20iLCJzY29wZSI6Imh0dHBzOi8vd3d3Lmdvb2dsZWFwaXMuY29tL2F1dGgvY2xvdWQtcGxhdGZvcm0ifQ.OtL40PSxdAB9rxRkXj-UeyuMhQCoT10WJY4ccOrPXriwm-DRl5AMgbBkQvVmWeYuPMTiFKWz_CMMBjVc3lFPW015eHvKT5r3ySGra1i8hJ9cDsWO7SdIGB-l00G-BdRxVEhN8U4C20eUhlvhtjXemOwlCFrKjF22rJB-ChiKy84rXs3O-Hz0dWmsSZPfVD9q-2S2vJdr9vz7NoP-fCmpxhQ3POVocYb-2OEM5c4Uo_e7lQTX3bRtVc19wz_wrTu9wMMMRYt52K8WPoWPURt7qpjHX88_EitXMzH-cJUQoDsgIoZ6vDlQMs7_nqNfgrlsGWHpPoSoGgvJMg1vJbzVLw)""";
// This "magic" assertion is generated in a similar manner, but specifies a
// non-default scope set and subject string (values used can be found in the
// kAltScopeForTest and kSubjectForGrant variables).
constexpr char kExpectedAssertionWithOptionalArgsParam[] =
    R"""(eyJhbGciOiJSUzI1NiIsImtpZCI6ImExYTExMWFhMTExMWExMWExMWExMWFhMTExYTExMWExYTExMTExMTEiLCJ0eXAiOiJKV1QifQ.eyJhdWQiOiJodHRwczovL29hdXRoMi5nb29nbGVhcGlzLmNvbS90b2tlbiIsImV4cCI6MTUzMDA2MzkyNCwiaWF0IjoxNTMwMDYwMzI0LCJpc3MiOiJmb28tZW1haWxAZm9vLXByb2plY3QuaWFtLmdzZXJ2aWNlYWNjb3VudC5jb20iLCJzY29wZSI6Imh0dHBzOi8vd3d3Lmdvb2dsZWFwaXMuY29tL2F1dGgvZGV2c3RvcmFnZS5mdWxsX2NvbnRyb2wiLCJzdWIiOiJ1c2VyQGZvby5iYXIifQ.D2sZntI1C0yF3LE3R0mssmidj8e9m5VU6UwzIUvDIG6yAxQLDRWK_gEdPW7etJ1xklIDwPEk0WgEsiu9pP89caPig0nK-bih7f1vbpRBTx4Vke07roW3DpFCLXFgaEXhKJYbzoYOJ62H_oBbQISC9qSF841sqEHmbjOqj5rSAR43wJm9H9juDT8apGpDNVCJM5pSo99NprLCvxUXuCBnacEsSQwbbZlLHfmBdyrllJsumx8RgFd22laEHsgPAMTxP-oM2iyf3fBEs2s1Dj7GxdWdpG6D9abJA6Hs8H1HqSwwyEWTXH6v_SPMYGsN1hIMTAWbO7J11bdHdjxo0hO5CA)""";
constexpr std::time_t kFixedJwtTimestamp = 1530060324;
constexpr char kGrantParamUnescaped[] =
    "urn:ietf:params:oauth:grant-type:jwt-bearer";
constexpr char kGrantParamEscaped[] =
    "urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Ajwt-bearer";
constexpr char kJsonKeyfileContents[] = R"""({
      "type": "service_account",
      "project_id": "foo-project",
      "private_key_id": "a1a111aa1111a11a11a11aa111a111a1a1111111",
      "private_key": "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCltiF2oP3KJJ+S\ntTc1McylY+TuAi3AdohX7mmqIjd8a3eBYDHs7FlnUrFC4CRijCr0rUqYfg2pmk4a\n6TaKbQRAhWDJ7XD931g7EBvCtd8+JQBNWVKnP9ByJUaO0hWVniM50KTsWtyX3up/\nfS0W2R8Cyx4yvasE8QHH8gnNGtr94iiORDC7De2BwHi/iU8FxMVJAIyDLNfyk0hN\neheYKfIDBgJV2v6VaCOGWaZyEuD0FJ6wFeLybFBwibrLIBE5Y/StCrZoVZ5LocFP\nT4o8kT7bU6yonudSCyNMedYmqHj/iF8B2UN1WrYx8zvoDqZk0nxIglmEYKn/6U7U\ngyETGcW9AgMBAAECggEAC231vmkpwA7JG9UYbviVmSW79UecsLzsOAZnbtbn1VLT\nPg7sup7tprD/LXHoyIxK7S/jqINvPU65iuUhgCg3Rhz8+UiBhd0pCH/arlIdiPuD\n2xHpX8RIxAq6pGCsoPJ0kwkHSw8UTnxPV8ZCPSRyHV71oQHQgSl/WjNhRi6PQroB\nSqc/pS1m09cTwyKQIopBBVayRzmI2BtBxyhQp9I8t5b7PYkEZDQlbdq0j5Xipoov\n9EW0+Zvkh1FGNig8IJ9Wp+SZi3rd7KLpkyKPY7BK/g0nXBkDxn019cET0SdJOHQG\nDiHiv4yTRsDCHZhtEbAMKZEpku4WxtQ+JjR31l8ueQKBgQDkO2oC8gi6vQDcx/CX\nZ23x2ZUyar6i0BQ8eJFAEN+IiUapEeCVazuxJSt4RjYfwSa/p117jdZGEWD0GxMC\n+iAXlc5LlrrWs4MWUc0AHTgXna28/vii3ltcsI0AjWMqaybhBTTNbMFa2/fV2OX2\nUimuFyBWbzVc3Zb9KAG4Y7OmJQKBgQC5324IjXPq5oH8UWZTdJPuO2cgRsvKmR/r\n9zl4loRjkS7FiOMfzAgUiXfH9XCnvwXMqJpuMw2PEUjUT+OyWjJONEK4qGFJkbN5\n3ykc7p5V7iPPc7Zxj4mFvJ1xjkcj+i5LY8Me+gL5mGIrJ2j8hbuv7f+PWIauyjnp\nNx/0GVFRuQKBgGNT4D1L7LSokPmFIpYh811wHliE0Fa3TDdNGZnSPhaD9/aYyy78\nLkxYKuT7WY7UVvLN+gdNoVV5NsLGDa4cAV+CWPfYr5PFKGXMT/Wewcy1WOmJ5des\nAgMC6zq0TdYmMBN6WpKUpEnQtbmh3eMnuvADLJWxbH3wCkg+4xDGg2bpAoGAYRNk\nMGtQQzqoYNNSkfus1xuHPMA8508Z8O9pwKU795R3zQs1NAInpjI1sOVrNPD7Ymwc\nW7mmNzZbxycCUL/yzg1VW4P1a6sBBYGbw1SMtWxun4ZbnuvMc2CTCh+43/1l+FHe\nMmt46kq/2rH2jwx5feTbOE6P6PINVNRJh/9BDWECgYEAsCWcH9D3cI/QDeLG1ao7\nrE2NcknP8N783edM07Z/zxWsIsXhBPY3gjHVz2LDl+QHgPWhGML62M0ja/6SsJW3\nYvLLIc82V7eqcVJTZtaFkuht68qu/Jn1ezbzJMJ4YXDYo1+KFi+2CAGR06QILb+I\nlUtj+/nH3HDQjM4ltYfTPUg=\n-----END PRIVATE KEY-----\n",
      "client_email": "foo-email@foo-project.iam.gserviceaccount.com",
      "client_id": "100000000000000000001",
      "auth_uri": "https://accounts.google.com/o/oauth2/auth",
      "token_uri": "https://oauth2.googleapis.com/token",
      "auth_provider_x509_cert_url": "https://www.googleapis.com/oauth2/v1/certs",
      "client_x509_cert_url": "https://www.googleapis.com/robot/v1/metadata/x509/foo-email%40foo-project.iam.gserviceaccount.com"
})""";
constexpr char kSubjectForGrant[] = "user@foo.bar";

class ServiceAccountCredentialsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mock_rest_client_ = absl::make_unique<MockRestClient>();
    testing_util::FakeClock::reset_clock(kFixedJwtTimestamp);
  }
  std::string CreateRandomFileName() {
    // When running on the internal Google CI systems we cannot write to the
    // local directory, GTest has a good temporary directory in that case.
    return google::cloud::internal::PathAppend(
        ::testing::TempDir(),
        google::cloud::internal::Sample(
            generator_, 8, "abcdefghijklmnopqrstuvwxyz0123456789"));
  }

  void CheckInfoYieldsExpectedAssertion(
      ServiceAccountCredentialsInfo const& info, std::string const& assertion);

  google::cloud::internal::DefaultPRNG generator_ =
      google::cloud::internal::MakeDefaultPRNG();
  std::unique_ptr<MockRestClient> mock_rest_client_;
};

void ServiceAccountCredentialsTest::CheckInfoYieldsExpectedAssertion(
    ServiceAccountCredentialsInfo const& info, std::string const& assertion) {
  std::string response = R"""({
      "token_type": "Type",
      "access_token": "access-token-value",
      "expires_in": 1234
  })""";

  auto* mock_response1 = new MockRestResponse();
  EXPECT_CALL(*mock_response1, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response1), ExtractPayload).WillOnce([&] {
    auto mock_http_payload = absl::make_unique<MockHttpPayload>();
    EXPECT_CALL(*mock_http_payload, Read)
        .WillOnce([&](absl::Span<char> buffer) {
          std::copy(response.begin(), response.end(), buffer.begin());
          return response.size();
        })
        .WillOnce([](absl::Span<char>) { return 0; });
    return std::unique_ptr<HttpPayload>(std::move(mock_http_payload));
  });

  EXPECT_CALL(
      *mock_rest_client_,
      Post(_, A<std::vector<std::pair<std::string, std::string>> const&>()))
      .WillOnce(
          [&](rest_internal::RestRequest const&,
              std::vector<std::pair<std::string, std::string>> const& payload) {
            EXPECT_THAT(payload, Contains(std::pair<std::string, std::string>(
                                     "assertion", assertion)));
            EXPECT_THAT(payload, Contains(std::pair<std::string, std::string>(
                                     "grant_type", kGrantParamUnescaped)));
            return std::unique_ptr<rest_internal::RestResponse>(mock_response1);
          });

  ServiceAccountCredentials credentials(info, {}, std::move(mock_rest_client_),
                                        FakeClock::now);
  // Calls Refresh to obtain the access token for our authorization header.
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Type access-token-value"}),
            credentials.AuthorizationHeader().value());
}

/// @test Verify that we can create service account credentials from a keyfile.
TEST_F(ServiceAccountCredentialsTest,
       RefreshingSendsCorrectRequestBodyAndParsesResponse) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  CheckInfoYieldsExpectedAssertion(*info, kExpectedAssertionParam);
}

/// @test Verify that we can create service account credentials from a keyfile.
TEST_F(ServiceAccountCredentialsTest,
       RefreshingSendsCorrectRequestBodyAndParsesResponseForNonDefaultVals) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  info->scopes = {kAltScopeForTest};
  info->subject = std::string(kSubjectForGrant);
  CheckInfoYieldsExpectedAssertion(*info,
                                   kExpectedAssertionWithOptionalArgsParam);
}

/// @test Verify that we refresh service account credentials appropriately.
TEST_F(ServiceAccountCredentialsTest,
       RefreshCalledOnlyWhenAccessTokenIsMissingOrInvalid) {
  // Prepare two responses, the first one is used but becomes immediately
  // expired, resulting in another refresh next time the caller tries to get
  // an authorization header.
  std::string r1 = R"""({
    "token_type": "Type",
    "access_token": "access-token-r1",
    "expires_in": 0
})""";
  std::string r2 = R"""({
    "token_type": "Type",
    "access_token": "access-token-r2",
    "expires_in": 1000
})""";

  auto* mock_response1 = new MockRestResponse();
  EXPECT_CALL(*mock_response1, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response1), ExtractPayload).WillOnce([&] {
    auto mock_http_payload = absl::make_unique<MockHttpPayload>();
    EXPECT_CALL(*mock_http_payload, Read)
        .WillOnce([&](absl::Span<char> buffer) {
          std::copy(r1.begin(), r1.end(), buffer.begin());
          return r1.size();
        })
        .WillOnce([](absl::Span<char>) { return 0; });
    return std::unique_ptr<HttpPayload>(std::move(mock_http_payload));
  });

  auto* mock_response2 = new MockRestResponse();
  EXPECT_CALL(*mock_response2, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response2), ExtractPayload).WillOnce([&] {
    auto mock_http_payload = absl::make_unique<MockHttpPayload>();
    EXPECT_CALL(*mock_http_payload, Read)
        .WillOnce([&](absl::Span<char> buffer) {
          std::copy(r2.begin(), r2.end(), buffer.begin());
          return r2.size();
        })
        .WillOnce([](absl::Span<char>) { return 0; });
    return std::unique_ptr<HttpPayload>(std::move(mock_http_payload));
  });

  EXPECT_CALL(
      *mock_rest_client_,
      Post(_, A<std::vector<std::pair<std::string, std::string>> const&>()))
      .WillOnce(
          [&](rest_internal::RestRequest const&,
              std::vector<std::pair<std::string, std::string>> const& payload) {
            EXPECT_THAT(payload, Contains(std::pair<std::string, std::string>(
                                     "grant_type", kGrantParamUnescaped)));
            return std::unique_ptr<rest_internal::RestResponse>(mock_response1);
          })
      .WillOnce(
          [&](rest_internal::RestRequest const&,
              std::vector<std::pair<std::string, std::string>> const& payload) {
            EXPECT_THAT(payload, Contains(std::pair<std::string, std::string>(
                                     "grant_type", kGrantParamUnescaped)));
            return std::unique_ptr<rest_internal::RestResponse>(mock_response2);
          });

  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  ServiceAccountCredentials credentials(*info, {},
                                        std::move(mock_rest_client_));
  // Calls Refresh to obtain the access token for our authorization header.
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Type access-token-r1"}),
            credentials.AuthorizationHeader().value());
  // Token is expired, resulting in another call to Refresh.
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Type access-token-r2"}),
            credentials.AuthorizationHeader().value());
  // Token still valid; should return cached token instead of calling Refresh.
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Type access-token-r2"}),
            credentials.AuthorizationHeader().value());
}

/// @test Verify that `nlohmann::json::parse()` failures are reported as
/// is_discarded.
TEST_F(ServiceAccountCredentialsTest, JsonParsingFailure) {
  std::string config = R"""( not-a-valid-json-string )""";
  // The documentation for `nlohmann::json::parse()` is a bit ambiguous, so
  // wrote a little test to verify it works as I expected.
  auto parsed = nlohmann::json::parse(config, nullptr, false);
  EXPECT_TRUE(parsed.is_discarded());
  EXPECT_FALSE(parsed.is_null());
}

/// @test Verify that parsing a service account JSON string works.
TEST_F(ServiceAccountCredentialsTest, ParseSimple) {
  std::string contents = R"""({
      "type": "service_account",
      "private_key_id": "not-a-key-id-just-for-testing",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com",
      "token_uri": "https://oauth2.googleapis.com/test_endpoint"
})""";

  auto actual =
      ParseServiceAccountCredentials(contents, "test-data", "unused-uri");
  ASSERT_STATUS_OK(actual);
  EXPECT_EQ("not-a-key-id-just-for-testing", actual->private_key_id);
  EXPECT_EQ("not-a-valid-key-just-for-testing", actual->private_key);
  EXPECT_EQ("test-only@test-group.example.com", actual->client_email);
  EXPECT_EQ("https://oauth2.googleapis.com/test_endpoint", actual->token_uri);
}

/// @test Verify that parsing a service account JSON string works.
TEST_F(ServiceAccountCredentialsTest, ParseUsesExplicitDefaultTokenUri) {
  // No token_uri attribute here, so the default passed below should be used.
  std::string contents = R"""({
      "type": "service_account",
      "private_key_id": "not-a-key-id-just-for-testing",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com"
})""";

  auto actual = ParseServiceAccountCredentials(
      contents, "test-data", "https://oauth2.googleapis.com/test_endpoint");
  ASSERT_STATUS_OK(actual);
  EXPECT_EQ("not-a-key-id-just-for-testing", actual->private_key_id);
  EXPECT_EQ("not-a-valid-key-just-for-testing", actual->private_key);
  EXPECT_EQ("test-only@test-group.example.com", actual->client_email);
  EXPECT_EQ("https://oauth2.googleapis.com/test_endpoint", actual->token_uri);
}

/// @test Verify that parsing a service account JSON string works.
TEST_F(ServiceAccountCredentialsTest, ParseUsesImplicitDefaultTokenUri) {
  // No token_uri attribute here.
  std::string contents = R"""({
      "type": "service_account",
      "private_key_id": "not-a-key-id-just-for-testing",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com"
})""";

  // No token_uri passed in here, either.
  auto actual = ParseServiceAccountCredentials(contents, "test-data");
  ASSERT_STATUS_OK(actual);
  EXPECT_EQ("not-a-key-id-just-for-testing", actual->private_key_id);
  EXPECT_EQ("not-a-valid-key-just-for-testing", actual->private_key);
  EXPECT_EQ("test-only@test-group.example.com", actual->client_email);
  EXPECT_EQ(std::string(GoogleOAuthRefreshEndpoint()), actual->token_uri);
}

/// @test Verify that invalid contents result in a readable error.
TEST_F(ServiceAccountCredentialsTest, ParseInvalidContentsFails) {
  std::string config = R"""( not-a-valid-json-string )""";

  auto actual = ParseServiceAccountCredentials(config, "test-as-a-source");
  EXPECT_THAT(actual,
              StatusIs(Not(StatusCode::kOk),
                       AllOf(HasSubstr("Invalid ServiceAccountCredentials"),
                             HasSubstr("test-as-a-source"))));
}

/// @test Parsing a service account JSON string should detect empty fields.
TEST_F(ServiceAccountCredentialsTest, ParseEmptyFieldFails) {
  std::string contents = R"""({
      "type": "service_account",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com",
      "token_uri": "https://oauth2.googleapis.com/token"
})""";

  for (auto const& field : {"private_key", "client_email", "token_uri"}) {
    auto json = nlohmann::json::parse(contents);
    json[field] = "";
    auto actual = ParseServiceAccountCredentials(json.dump(), "test-data", "");
    EXPECT_THAT(actual,
                StatusIs(Not(StatusCode::kOk),
                         AllOf(HasSubstr(field), HasSubstr(" field is empty"),
                               HasSubstr("test-data"))));
  }
}

/// @test Parsing a service account JSON string should detect missing fields.
TEST_F(ServiceAccountCredentialsTest, ParseMissingFieldFails) {
  std::string contents = R"""({
      "type": "service_account",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com",
      "token_uri": "https://oauth2.googleapis.com/token"
})""";

  for (auto const& field : {"private_key", "client_email"}) {
    auto json = nlohmann::json::parse(contents);
    json.erase(field);
    auto actual = ParseServiceAccountCredentials(json.dump(), "test-data", "");
    EXPECT_THAT(actual,
                StatusIs(Not(StatusCode::kOk),
                         AllOf(HasSubstr(field), HasSubstr(" field is missing"),
                               HasSubstr("test-data"))));
  }
}

/// @test Parsing a service account JSON string allows an optional field.
TEST_F(ServiceAccountCredentialsTest, ParseOptionalField) {
  std::string contents = R"""({
      "type": "service_account",
      "private_key_id": "",
      "private_key": "not-a-valid-key-just-for-testing",
      "client_email": "test-only@test-group.example.com",
      "token_uri": "https://oauth2.googleapis.com/token"
})""";

  auto json = nlohmann::json::parse(contents);
  auto actual = ParseServiceAccountCredentials(json.dump(), "test-data", "");
  ASSERT_STATUS_OK(actual.status());
}

/// @test Verify that refreshing a credential updates the timestamps.
TEST_F(ServiceAccountCredentialsTest, RefreshingUpdatesTimestamps) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);

  // NOLINTNEXTLINE(google-runtime-int)
  auto request_assertion =
      [&info](int timestamp,
              std::vector<std::pair<std::string, std::string>> const& p) {
        EXPECT_THAT(p, Contains(std::pair<std::string, std::string>(
                           "grant_type", kGrantParamUnescaped)));

        auto assertion =
            std::find_if(p.begin(), p.end(),
                         [](std::pair<std::string, std::string> const& e) {
                           return e.first == "assertion";
                         });
        ASSERT_NE(assertion, p.end());

        std::vector<std::string> const tokens =
            absl::StrSplit(assertion->second, '.');
        std::string const& encoded_header = tokens[0];
        std::string const& encoded_payload = tokens[1];

        auto header_bytes =
            internal::UrlsafeBase64Decode(encoded_header).value();
        std::string header_str{header_bytes.begin(), header_bytes.end()};
        auto payload_bytes =
            internal::UrlsafeBase64Decode(encoded_payload).value();
        std::string payload_str{payload_bytes.begin(), payload_bytes.end()};

        auto header = nlohmann::json::parse(header_str);
        EXPECT_EQ("RS256", header.value("alg", ""));
        EXPECT_EQ("JWT", header.value("typ", ""));
        EXPECT_EQ(info->private_key_id, header.value("kid", ""));

        auto payload = nlohmann::json::parse(payload_str);
        EXPECT_EQ(timestamp, payload.value("iat", 0));
        EXPECT_EQ(timestamp + 3600, payload.value("exp", 0));
        EXPECT_EQ(info->client_email, payload.value("iss", ""));
        EXPECT_EQ(info->token_uri, payload.value("aud", ""));
      };

  // Setup the mock request / response for the first Refresh().
  auto const clock_value_1 = 10000;
  auto const clock_value_2 = 20000;

  auto response_fn = [&](int timestamp, absl::Span<char>& buffer) {
    std::string token = "mock-token-value-" + std::to_string(timestamp);
    nlohmann::json response{{"token_type", "Mock-Type"},
                            {"access_token", token},
                            {"expires_in", 3600}};
    auto r = response.dump();
    std::copy(r.begin(), r.end(), buffer.begin());
    return r.size();
  };

  auto* mock_response1 = new MockRestResponse();
  EXPECT_CALL(*mock_response1, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response1), ExtractPayload).WillOnce([&] {
    auto mock_http_payload = absl::make_unique<MockHttpPayload>();
    EXPECT_CALL(*mock_http_payload, Read)
        .WillOnce([&](absl::Span<char> buffer) {
          return response_fn(clock_value_1, buffer);
        })
        .WillOnce([](absl::Span<char>) { return 0; });
    return std::unique_ptr<HttpPayload>(std::move(mock_http_payload));
  });

  auto* mock_response2 = new MockRestResponse();
  EXPECT_CALL(*mock_response2, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response2), ExtractPayload).WillOnce([&] {
    auto mock_http_payload = absl::make_unique<MockHttpPayload>();
    EXPECT_CALL(*mock_http_payload, Read)
        .WillOnce([&](absl::Span<char> buffer) {
          return response_fn(clock_value_2, buffer);
        })
        .WillOnce([](absl::Span<char>) { return 0; });
    return std::unique_ptr<HttpPayload>(std::move(mock_http_payload));
  });

  EXPECT_CALL(
      *mock_rest_client_,
      Post(_, A<std::vector<std::pair<std::string, std::string>> const&>()))
      .WillOnce(
          [&](rest_internal::RestRequest const&,
              std::vector<std::pair<std::string, std::string>> const& payload) {
            request_assertion(clock_value_1, payload);
            return std::unique_ptr<rest_internal::RestResponse>(mock_response1);
          })
      .WillOnce(
          [&](rest_internal::RestRequest const&,
              std::vector<std::pair<std::string, std::string>> const& payload) {
            request_assertion(clock_value_2, payload);
            return std::unique_ptr<rest_internal::RestResponse>(mock_response2);
          });

  FakeClock::now_value_ = clock_value_1;
  ServiceAccountCredentials credentials(*info, {}, std::move(mock_rest_client_),
                                        FakeClock::now);
  // Call Refresh to obtain the access token for our authorization header.
  auto authorization_header = credentials.AuthorizationHeader();
  ASSERT_STATUS_OK(authorization_header);
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Mock-Type mock-token-value-10000"}),
            authorization_header.value());

  // Advance the clock past the expiration time of the token and then get a
  // new header.
  FakeClock::now_value_ = clock_value_2;
  EXPECT_GT(clock_value_2 - clock_value_1, 2 * 3600);
  authorization_header = credentials.AuthorizationHeader();
  ASSERT_STATUS_OK(authorization_header);
  EXPECT_EQ(std::make_pair(std::string{"Authorization"},
                           std::string{"Mock-Type mock-token-value-20000"}),
            authorization_header.value());
}

/// @test Verify that we can create sign blobs using a service account.
TEST_F(ServiceAccountCredentialsTest, SignBlob) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  ServiceAccountCredentials credentials(*info);

  std::string blob = R"""(GET
rmYdCNHKFXam78uCt7xQLw==
text/plain
1388534400
x-goog-encryption-algorithm:AES256
x-goog-meta-foo:bar,baz
/bucket/objectname)""";

  auto actual = credentials.SignBlob(info->client_email, blob);
  ASSERT_STATUS_OK(actual);

  // To generate the expected output I used:
  //   `openssl dgst -sha256 -sign private.pem blob.txt | openssl base64 -A`
  // where `blob.txt` contains the `blob` string, and `private.pem` contains
  // the private key embedded in `kJsonKeyfileContents`.
  std::string expected_signed =
      "Zsy8o5ci07DQTvO/"
      "SVr47PKsCXvN+"
      "FzXga0iYrReAnngdZYewHdcAnMQ8bZvFlTM8HY3msrRw64Jc6hoXVL979An5ugXoZ1ol/"
      "DT1KlKp3l9E0JSIbqL88ogpElTxFvgPHOtHOUsy2mzhqOVrNSXSj4EM50gKHhvHKSbFq8Pcj"
      "lAkROtq5gqp5t0OFd7EMIaRH+tekVUZjQPfFT/"
      "hRW9bSCCV8w1Ex+"
      "QxmB5z7P7zZn2pl7JAcL850emTo8f2tfv1xXWQGhACvIJeMdPmyjbc04Ye4M8Ljpkg3YhE6l"
      "4GwC2MnI8TkuoHe4Bj2MvA8mM8TVwIvpBs6Etsj6Jdaz4rg==";
  internal::Base64Encoder encoder;
  for (auto const& c : *actual) {
    encoder.PushBack(c);
  }
  EXPECT_EQ(expected_signed, std::move(encoder).FlushAndPad());
}

/// @test Verify that signing blobs fails with invalid e-mail.
TEST_F(ServiceAccountCredentialsTest, SignBlobFailure) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  ServiceAccountCredentials credentials(*info);

  auto actual = credentials.SignBlob("fake@fake.com", "test-blob");
  EXPECT_THAT(
      actual,
      StatusIs(StatusCode::kInvalidArgument,
               HasSubstr("The current_credentials cannot sign blobs for ")));
}

/// @test Verify that we can get the client id from a service account.
TEST_F(ServiceAccountCredentialsTest, ClientId) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  ServiceAccountCredentials credentials(*info);

  EXPECT_EQ("foo-email@foo-project.iam.gserviceaccount.com",
            credentials.AccountEmail());
  EXPECT_EQ("a1a111aa1111a11a11a11aa111a111a1a1111111", credentials.KeyId());
}

/// @test Verify we can obtain JWT assertion components given the info parsed
/// from a keyfile.
TEST_F(ServiceAccountCredentialsTest, AssertionComponentsFromInfo) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  auto const clock_value_1 = 10000;
  FakeClock::now_value_ = clock_value_1;
  auto components = AssertionComponentsFromInfo(*info, FakeClock::now());

  auto header = nlohmann::json::parse(components.first);
  EXPECT_EQ("RS256", header.value("alg", ""));
  EXPECT_EQ("JWT", header.value("typ", ""));
  EXPECT_EQ(info->private_key_id, header.value("kid", ""));

  auto payload = nlohmann::json::parse(components.second);
  EXPECT_EQ(clock_value_1, payload.value("iat", 0));
  EXPECT_EQ(clock_value_1 + 3600, payload.value("exp", 0));
  EXPECT_EQ(info->client_email, payload.value("iss", ""));
  EXPECT_EQ(info->token_uri, payload.value("aud", ""));
}

/// @test Verify we can construct a JWT assertion given the info parsed from a
/// keyfile.
TEST_F(ServiceAccountCredentialsTest, MakeJWTAssertion) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  FakeClock::reset_clock(kFixedJwtTimestamp);
  auto components = AssertionComponentsFromInfo(*info, FakeClock::now());
  auto assertion =
      MakeJWTAssertion(components.first, components.second, info->private_key);

  std::vector<std::string> expected_tokens =
      absl::StrSplit(kExpectedAssertionParam, '.');
  std::string const& expected_encoded_header = expected_tokens[0];
  std::string const& expected_encoded_payload = expected_tokens[1];
  std::string const& expected_encoded_signature = expected_tokens[2];

  std::vector<std::string> actual_tokens = absl::StrSplit(assertion, '.');
  std::string const& actual_encoded_header = actual_tokens[0];
  std::string const& actual_encoded_payload = actual_tokens[1];
  std::string const& actual_encoded_signature = actual_tokens[2];

  EXPECT_EQ(expected_encoded_header, actual_encoded_header);
  EXPECT_EQ(expected_encoded_payload, actual_encoded_payload);
  EXPECT_EQ(expected_encoded_signature, actual_encoded_signature);
}

/// @test Verify we can construct a service account refresh payload given the
/// info parsed from a keyfile.
TEST_F(ServiceAccountCredentialsTest, CreateServiceAccountRefreshPayload) {
  auto info = ParseServiceAccountCredentials(kJsonKeyfileContents, "test");
  ASSERT_STATUS_OK(info);
  FakeClock::reset_clock(kFixedJwtTimestamp);
  auto components = AssertionComponentsFromInfo(*info, FakeClock::now());
  auto assertion =
      MakeJWTAssertion(components.first, components.second, info->private_key);
  auto actual_payload = CreateServiceAccountRefreshPayload(
      *info, std::make_pair("grant_type", kGrantParamEscaped),
      FakeClock::now());

  EXPECT_THAT(actual_payload, Contains(std::pair<std::string, std::string>(
                                  "assertion", assertion)));
  EXPECT_THAT(actual_payload, Contains(std::pair<std::string, std::string>(
                                  "grant_type", kGrantParamEscaped)));
}

/// @test Parsing a refresh response with missing fields results in failure.
TEST_F(ServiceAccountCredentialsTest,
       ParseServiceAccountRefreshResponseMissingFields) {
  std::string r1 = R"""({})""";
  // Does not have access_token.
  std::string r2 = R"""({
    "token_type": "Type",
    "id_token": "id-token-value",
    "expires_in": 1000
})""";

  std::unique_ptr<MockHttpPayload> mock_http_payload1(new MockHttpPayload());
  EXPECT_CALL(*mock_http_payload1, Read)
      .WillOnce([&](absl::Span<char> buffer) {
        std::copy(r1.begin(), r1.end(), buffer.begin());
        return r1.size();
      })
      .WillOnce([](absl::Span<char>) { return 0; });

  auto mock_response1 = absl::make_unique<MockRestResponse>();
  EXPECT_CALL(*mock_response1, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kBadRequest));
  EXPECT_CALL(std::move(*mock_response1), ExtractPayload)
      .WillOnce(Return(ByMove(std::move(mock_http_payload1))));

  std::unique_ptr<MockHttpPayload> mock_http_payload2(new MockHttpPayload());
  EXPECT_CALL(*mock_http_payload2, Read)
      .WillOnce([&](absl::Span<char> buffer) {
        std::copy(r2.begin(), r2.end(), buffer.begin());
        return r2.size();
      })
      .WillOnce([](absl::Span<char>) { return 0; });

  auto mock_response2 = absl::make_unique<MockRestResponse>();
  EXPECT_CALL(*mock_response2, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kBadRequest));
  EXPECT_CALL(std::move(*mock_response2), ExtractPayload)
      .WillOnce(Return(ByMove(std::move(mock_http_payload2))));

  FakeClock::reset_clock(1000);
  auto status =
      ParseServiceAccountRefreshResponse(*mock_response1, FakeClock::now());
  EXPECT_THAT(status,
              StatusIs(StatusCode::kInvalidArgument,
                       HasSubstr("Could not find all required fields")));

  status =
      ParseServiceAccountRefreshResponse(*mock_response2, FakeClock::now());
  EXPECT_THAT(status,
              StatusIs(StatusCode::kInvalidArgument,
                       HasSubstr("Could not find all required fields")));
}

/// @test Parsing a refresh response yields a TemporaryToken.
TEST_F(ServiceAccountCredentialsTest, ParseServiceAccountRefreshResponse) {
  std::string r1 = R"""({
    "token_type": "Type",
    "access_token": "access-token-r1",
    "expires_in": 1000
})""";

  std::unique_ptr<MockHttpPayload> mock_http_payload1(new MockHttpPayload());
  EXPECT_CALL(*mock_http_payload1, Read)
      .WillOnce([&](absl::Span<char> buffer) {
        std::copy(r1.begin(), r1.end(), buffer.begin());
        return r1.size();
      })
      .WillOnce([](absl::Span<char>) { return 0; });

  auto mock_response1 = absl::make_unique<MockRestResponse>();
  EXPECT_CALL(*mock_response1, StatusCode)
      .WillRepeatedly(Return(rest_internal::HttpStatusCode::kOk));
  EXPECT_CALL(std::move(*mock_response1), ExtractPayload)
      .WillOnce(Return(ByMove(std::move(mock_http_payload1))));

  auto expires_in = 1000;
  FakeClock::reset_clock(2000);
  auto status =
      ParseServiceAccountRefreshResponse(*mock_response1, FakeClock::now());
  EXPECT_STATUS_OK(status);
  auto token = *status;
  EXPECT_EQ(
      std::chrono::time_point_cast<std::chrono::seconds>(token.expiration_time)
          .time_since_epoch()
          .count(),
      FakeClock::now_value_ + expires_in);
  EXPECT_EQ(token.token, std::make_pair(std::string{"Authorization"},
                                        std::string{"Type access-token-r1"}));
}

}  // namespace
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace oauth2_internal
}  // namespace cloud
}  // namespace google
