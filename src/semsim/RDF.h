//
// Created by Ciaran on 5/31/2020.
//

#ifndef LIBSEMSIM_RDF_H
#define LIBSEMSIM_RDF_H

#include <unordered_map>
#include "RedlandAPIWrapper.h"
#include "semsim/SemsimUtils.h"
#include "Editor.h"

using namespace redland;

namespace semsim {


    class RDF {
        LibrdfStorage storage_;
        LibrdfModel model_;

        typedef std::unordered_map<std::string, std::string> NamespaceMap;

    public:
        std::string base_uri_;
        NamespaceMap namespaces_;
        std::vector<std::string> seen_namespaces_;

        // todo remove this field and replace with the one
        //  in Predicate. Should be a simple swap.
        NamespaceMap default_namespaces_ = {
                {"http://purl.org/dc/terms/",                "dcterms"},
                {"http://biomodels.net/biology-qualifiers/", "bqbiol"},
                {"http://biomodels.net/model-qualifiers/",   "bqmodel"},
                {"http://www.bhi.washington.edu/semsim#",    "semsim"},
        };

        explicit RDF(const std::string &base_uri = "./Annotations.rdf", const std::string &storage_type = "memory",
                     const std::string &storage_name = "SemsimStore",
                     const char *storage_options = nullptr, const char *model_options = nullptr);

        void freeRDF();

        ~RDF();

        RDF(const RDF &rdf) = delete;

        RDF(RDF &&rdf) noexcept;

        RDF &operator=(const RDF &rdf) = delete;

        RDF &operator=(RDF &&rdf) noexcept;

        int size() const;

        void setBaseUri(std::string baseUri);

        bool empty() const;

        static RDF fromString(const std::string &str,
                              const std::string &format = "guess",
                              const std::string &base_uri = std::string());

        RDF addFromString(const std::string &str,
                       const std::string &format = "guess");

        /*
         * @brief non static version of RDF::fromString that takes
         * a * RDF pointer object and modifies in place.
         *
         * Primarily used for C API
         *
         */
        static void
        fromString(RDF *rdf, const std::string &str, const std::string &format, const std::string &base_uri);

        std::unordered_map<std::string, std::string>
        propagateNamespacesFromParser(const std::vector<std::string> &seen_namespaces);

        std::string toString(const std::string &format = "rdfxml-abbrev", std::string base_uri = std::string(),
                             const char *mime_type = nullptr, const char *type_uri = nullptr);

        Editor toEditor(const std::string &xml, SemsimXmlType type);

        Editor *toEditorPtr(const std::string &xml, SemsimXmlType type);

        librdf_model *getModel() const;

        librdf_storage *getStorage() const;

    };
}

#endif //LIBSEMSIM_RDF_H
