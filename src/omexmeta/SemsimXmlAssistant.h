//
// Created by Ciaran on 4/14/2020.
//

#ifndef LIBOMEXMETA_SEMSIMXMLASSISTANT_H
#define LIBOMEXMETA_SEMSIMXMLASSISTANT_H


#include <vector>
#include "iostream"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <memory>
#include "MetaID.h"
#include <utility>
#include "SemsimUtils.h"

namespace semsim {
    enum SemsimXmlType {
        SEMSIM_TYPE_SBML,
        SEMSIM_TYPE_CELLML,
        SEMSIM_TYPE_OTHER,
    };

    class SemsimXmlAssistant {
        std::string xml_;
        std::string metaid_base;
        int metaid_num_digits_;

        void addMetaIdsRecursion(xmlNode *a_node, std::vector<std::string> &seen_metaids);

        void generateMetaId(
                std::vector<std::string> &seen_metaids, long count,
                const MetaID &metaid_gen, std::string &id);

    public:

        explicit SemsimXmlAssistant(std::string xml, std::string base = "MetaID", int metaid_num_digits = 4);

        std::pair<std::string, std::vector<std::string>> addMetaIds();

        [[nodiscard]] virtual std::vector<std::string> getValidElements() const;
    };


    class SBMLAssistant : public SemsimXmlAssistant {

    public:
        std::vector<std::string> getValidElements() const override;

        using SemsimXmlAssistant::SemsimXmlAssistant;


    };


    class CellMLAssistant : public SemsimXmlAssistant {
    public:
        using SemsimXmlAssistant::SemsimXmlAssistant;

        std::vector<std::string> getValidElements() const override;

    };

    typedef std::unique_ptr<SemsimXmlAssistant> XmlAssistantPtr;

    class SemsimXmlAssistantFactory {
    public:

        static XmlAssistantPtr generate(const std::string &xml, SemsimXmlType type);
    };
}
#endif //LIBOMEXMETA_SEMSIMXMLASSISTANT_H