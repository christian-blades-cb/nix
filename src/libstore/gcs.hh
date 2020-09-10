#pragma once

#if HAVE_GCS

#include "ref.hh"

namespace google {
namespace cloud {
namespace storage {
namespace v1 {
class Client;
}
} // namespace v1
} // namespace cloud
}; // namespace google

// namespace gcs = google::cloud::storage;

namespace nix {

struct GCSHelper {
    ref<google::cloud::storage::v1::Client> client;

    GCSHelper();

    std::string download(const std::string & bucket, const std::string & key);

    void upload(const std::string & bucket, const std::string & key, const std::string & payload);
};

}

#endif
