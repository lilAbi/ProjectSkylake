#include "json_content_source.h"

CatalogData JSONContentSource::load() {
    CatalogData catalog_data;
    const Manifest manifest = this->load_manifest();

    for (const auto& source : manifest.sources) {
        this->load_source(source, data);
    }

    return catalog_data;
}