//
// Created by Ciaran on 4/22/2020.
//

#ifndef LIBSEMSIM_PHYSICALFORCE_H
#define LIBSEMSIM_PHYSICALFORCE_H


#include "PhysicalPhenomenon.h"
#include "Participant.h"
#include "librdf.h"


#include "PhysicalPhenomenon.h"
#include "Participant.h"
#include "RedlandAPIWrapper.h"
#include <vector>
#include "Participant.h"
#include "PhysicalPropertyResource.h"
#include "PhysicalPhenomenon.h"
#include "SemsimUtils.h"

using namespace redland;

namespace semsim {
    class PhysicalForce : public PhysicalPhenomenon {

        Sources sources_;
        Sinks sinks_;

    public:

        PhysicalForce() = delete;

        ~PhysicalForce() = default;


        PhysicalForce(librdf_model* model, Subject metaid, PhysicalPropertyResource physicalProperty,
                      Sources sources, Sinks sinks);
        /*
         * @brief Free nodes associated with PhysicalForce.
         *
         * The PhysicalForce is
         * owned by the caller if the PhysicalForce::toTriples method is NOT used.
         * When toTriples() is used, the nodes that create the PhysicalForce are "given"
         * to the Triples object, which automatically destroys them at the right time.
         *
         * Most of the time, users will not have to remember to free the PhysicalForce themselves
         * as the toTriples method is always used.
         *
         */
        void free() override;

        /*
         * @brief constructor for instantiating a PhysicalForce type composite annotation
         * @param model. A librdf_model pass down by Editor.
         *
         * Users do not need to instantiate PhysicalForce manually, since it is done
         * by Editor. This constructor instantiates an empty PhysicalForce
         * object which is filled by
         *
         */
        explicit PhysicalForce(librdf_model* model);

        /*
         * @brief create a metaid for the physical force annotation
         */
        [[nodiscard]] std::string createMetaId() const;

        /*
         * @brief return vector of Source objects assocated with PhysicalForce
         */
        [[nodiscard]] const Sources &getSources() const;

        /*
         * @brief return vector of Sink objects assocated with PhysicalForce
         */
        [[nodiscard]] const Sinks &getSinks() const;

        /*
         * @brief converts the PhyicalForce object into a Triples object.
         *
         * When this method is called ownership of all RDF nodes gets transferred from
         * the caller to the returned Triples object.
         */
        [[nodiscard]] Triples toTriples() override;

        /*
         * @brief sets the about field of the PhysicalForce
         * @param metaid id for the physical force. Result is relative to base_uri.
         *
         * A Resource object with a URI node is instantiated using the
         * the @param metaid.
         *
         * See Resource for more information about input format for @param metaid
         */
        PhysicalForce &setAbout(const std::string& metaid);

        /*
         * @brief sets the physical property of the PhysicalForce
         * @parameter physicalProperty An instance of PhysicalPropertyResource representing the
         * physical property term for the PhysicalForce.
         *
         * Prefer the other setPhysicalProperty method since it only requires a
         * string input and instantiates the PhysicalPropertyResource for you.
         *
         * For developers. Consider removing.
         */
        [[maybe_unused]] PhysicalForce &setPhysicalProperty(PhysicalPropertyResource physicalProperty);

        /*
         * @brief sets the physical property of the PhysicalForce
         * @parameter physicalProperty. An instance of PhysicalPropertyResource
         * representing the physical property term for the PhysicalForce is automatically instantiated
         */
        PhysicalForce &setPhysicalProperty(const std::string &physicalProperty);

        /*
         * @brief add a SourceParticipant to the PhysicalForce.
         * @param metaid the ID for the SourceParticipant.
         * @param multiplier The multiplier argument for SourceParticipant
         * @param physical_entity_reference The physicalEntityReference argument for
         * the SourceParticipant.
         *
         * See SourceParticipant documentation for more details on arguments.
         */
        PhysicalForce &addSource(std::string source_metaid, double multiplier,
                                 std::string physical_entity_reference);
        /*
         * @brief add a SinkParticipant to the PhysicalForce.
         * @param sink_metaid the ID for the SinkParticipant.
         * @param multiplier The multiplier argument for SinkParticipant
         * @param physical_entity_reference The physicalEntityReference argument for
         * the SinkParticipant.
         *
         * See SinkParticipant documentation for more details on arguments.
         */
        PhysicalForce &
        addSink(std::string sink_metaid, double multiplier,
                std::string physical_entity_reference);

        /*
         * @brief returns the number of sources associated with the
         * PhysicalForce
         */
        int getNumSources();

        /*
         * @brief returns the number of sinks associated with the
         * PhysicalForce
         */
        int getNumSinks();
    };
}

#endif //LIBSEMSIM_PHYSICALFORCE_H
