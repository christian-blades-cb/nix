#if HAVE_GCS

#include <google/cloud/storage/client.h>
#include <google/cloud/status.h>
#include <google/cloud/status_or.h>
#include "gcs.hh"
#include "ref.hh"
#include "filetransfer.hh"

namespace gcs = google::cloud::storage;

namespace nix {

static void throwForStatus(const google::cloud::Status & status)
{
    switch (status.code())
    {
    case google::cloud::StatusCode::kOk :
        return;
    case google::cloud::StatusCode::kNotFound :
        throw FileTransferError(FileTransfer::Error::NotFound, nullptr);
        break;
    case google::cloud::StatusCode::kPermissionDenied :
    case google::cloud::StatusCode::kUnauthenticated :
        throw FileTransferError(FileTransfer::Error::Forbidden, nullptr);
        break;
    default:
        throw FileTransferError(FileTransfer::Error::Misc, nullptr);
    }
};

GCSHelper::GCSHelper()
    : client(
        make_ref<google::cloud::storage::v1::Client>(google::cloud::storage::v1::Client::CreateDefaultClient().value())) {
};

std::string GCSHelper::download(const std::string & bucket, const std::string & key)
{
    gcs::ObjectReadStream stream = client->ReadObject(bucket, key);
    if (!stream.status().ok()) {
        throwForStatus(stream.status());
    }

    std::string payload(std::istreambuf_iterator<char>{stream}, {});
    return payload;
};

void GCSHelper::upload(const std::string & bucket, const std::string & key, const std::string & payload)
{
    google::cloud::v0::StatusOr<gcs::ObjectMetadata> object_metadata =
        client->InsertObject(bucket, key, std::move(payload));

    if (!object_metadata.ok()) {
        throwForStatus(object_metadata.status());
    }
};


}
#endif
