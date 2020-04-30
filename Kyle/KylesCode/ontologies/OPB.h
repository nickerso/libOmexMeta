# ifndef SEMSIM_OPB_H_
# define SEMSIM_OPB_H_


# include "Ontology.h"
// # include "semsim/PhysicalProperty.h"

namespace semsim {
    /**
     * Class for the <a href="http://bioportal.bioontology.org/ontologies/OPB">Ontology of Physics for Biology</a>.
     */
    class OPB : public Ontology {
    public:

        /**
         * Get the full URI of an ontology term given its numeric value.
         * @param  t The numeric value of the ontology term.
         * @return   The correct full URI for the resource in OPB.
         *
         * @code{.cpp}
         * // returns "https://identifiers.org/opb/OPB_00154" (fluid volume)
         * Resource r = OPB::get(154);
         * @endcode
         */
        static Resource get(OntologyTerm t) {
            return resolve("https://identifiers.org/opb/OPB_", t, 5);
        }

    };
}

# endif