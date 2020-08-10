//
// Created by Ciaran on 10/08/2020.
//


#include "gtest/gtest.h"
#include "omexmeta/RDF.h"

using namespace omexmeta;

class SBMLSemanticExtractionTests : public ::testing::TestCase {
public:
    std::string sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<!-- Created by libAntimony version v2.12.0 with libSBML version 5.18.1. -->\n"
                       "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
                       "    <model metaid=\"ToyModel\" id=\"ToyModel\">\n"
                       "        <listOfCompartments>\n"
                       "            <compartment id=\"cytosol\" metaid=\"comp1\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
                       "            <compartment id=\"extraCell\" metaid=\"comp2\" spatialDimensions=\"3\" size=\"10\" constant=\"true\"/>\n"
                       "        </listOfCompartments>\n"
                       "        <listOfSpecies>\n"
                       "            <species id=\"A\" metaid=\"sp_1\" compartment=\"cytosol\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\"\n"
                       "                     boundaryCondition=\"false\" constant=\"false\">\n"
                       "                <annotation>\n"
                       "                    <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                       "                             xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\">\n"
                       "                        <rdf:Description rdf:about=\"#_1\">\n"
                       "                            <bqbiol:is>\n"
                       "                                <rdf:Bag>\n"
                       "                                    <rdf:li rdf:resource=\"http://identifiers.org/uniprot/P00742\"/>\n"
                       "                                </rdf:Bag>\n"
                       "                            </bqbiol:is>\n"
                       "                        </rdf:Description>\n"
                       "                    </rdf:RDF>\n"
                       "                </annotation>\n"
                       "            </species>\n"
                       "\n"
                       "            <species id=\"B\" metaid=\"sp_2\" compartment=\"cytosol\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\"\n"
                       "                     boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "            <species id=\"Ca\" metaid=\"sp_3\" compartment=\"cytosol\" initialConcentration=\"2\" hasOnlySubstanceUnits=\"false\"\n"
                       "                     boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "            <species id=\"PlasmaCa\" metaid=\"sp_4\" compartment=\"extraCell\" initialConcentration=\"3\"\n"
                       "                     hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "            <species id=\"Enzyme\" metaid=\"sp_5\" compartment=\"cytosol\" initialConcentration=\"2\"\n"
                       "                     hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                       "\n"
                       "        </listOfSpecies>\n"
                       "        <listOfParameters>\n"
                       "            <parameter id=\"k1\" value=\"0.1\" constant=\"true\"/>\n"
                       "            <parameter id=\"k2\" value=\"0.1\" constant=\"true\"/>\n"
                       "        </listOfParameters>\n"
                       "        <listOfReactions>\n"
                       "            <reaction id=\"r1\" metaid=\"react1\" reversible=\"false\" fast=\"false\">\n"
                       "                <listOfReactants>\n"
                       "                    <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "                </listOfReactants>\n"
                       "                <listOfProducts>\n"
                       "                    <speciesReference species=\"A\" stoichiometry=\"2\" constant=\"true\"/>\n"
                       "                </listOfProducts>\n"
                       "                <kineticLaw>\n"
                       "                    <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "                        <apply>\n"
                       "                            <times/>\n"
                       "                            <ci>k2</ci>\n"
                       "                            <ci>B</ci>\n"
                       "                        </apply>\n"
                       "                    </math>\n"
                       "                </kineticLaw>\n"
                       "            </reaction>\n"
                       "            <reaction id=\"r2\" metaid=\"react2\" reversible=\"false\" fast=\"false\">\n"
                       "                <listOfReactants>\n"
                       "                    <speciesReference species=\"Ca\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "                    <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "                </listOfReactants>\n"
                       "                <listOfProducts>\n"
                       "                    <speciesReference species=\"PlasmaCa\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "                </listOfProducts>\n"
                       "                <listOfModifiers>\n"
                       "                    <modifierSpeciesReference species=\"Enzyme\">\n"
                       "                </listOfModifiers>\n"
                       "                <kineticLaw>\n"
                       "                    <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "                        <apply>\n"
                       "                            <times/>\n"
                       "                            <ci>k2</ci>\n"
                       "                            <ci>Ca</ci>\n"
                       "                        </apply>\n"
                       "                    </math>\n"
                       "                </kineticLaw>\n"
                       "            </reaction>\n"
                       "        </listOfReactions>\n"
                       "    </model>\n"
                       "</sbml>\n"
                       "";

    SBMLSemanticExtractionTests() = default;

}


TEST_F(SBMLSemanticExtractionTests, CompartmentExtractionTest1){

    // 1) I need some sbml recognition code in RDF. Look for several of the tags
    //    and ensure they are there, then switch a flag to sbml. This can then be propagated
    //    into Editor so the user will not have to tell omexmeta they are using sbml.
    //    We'll need similar with cellml and default to unknown.
    // 2) We need to iterate over the xml tree, pulling out elements with a tag that we want.
    //      This means I need a tool for doing so. Create a simple grabber function that will
    //      work its way through the tree and collect elements into a vector.
    // 3) Use the grabber function to pull out the elements we need for semantic extraction,
    //    create triples and add them to the model. Then inclde this in the constructor of RDF.
    //
    RDF rdf = RDF::fromString(sbml, "rdfxml");

    std::string expected =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                            "<!-- Created by libAntimony version v2.12.0 with libSBML version 5.18.1. -->\n"
                            "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
                            "    <model metaid=\"ToyModel\" id=\"ToyModel\">\n"
                            "        <listOfCompartments>\n"
                            "            <compartment id=\"cytosol\" metaid=\"comp1\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n"
                            "            <compartment id=\"extraCell\" metaid=\"comp2\" spatialDimensions=\"3\" size=\"10\" constant=\"true\"/>\n"
                            "        </listOfCompartments>\n"
                            "        <listOfSpecies>\n"
                            "            <species id=\"A\" metaid=\"sp_1\" compartment=\"cytosol\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\"\n"
                            "                     boundaryCondition=\"false\" constant=\"false\">\n"
                            "                <annotation>\n"
                            "                    <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                            "                             xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\">\n"
                            "                        <rdf:Description rdf:about=\"#_1\">\n"
                            "                            <bqbiol:is>\n"
                            "                                <rdf:Bag>\n"
                            "                                    <rdf:li rdf:resource=\"http://identifiers.org/uniprot/P00742\"/>\n"
                            "                                </rdf:Bag>\n"
                            "                            </bqbiol:is>\n"
                            "                        </rdf:Description>\n"
                            "                    </rdf:RDF>\n"
                            "                </annotation>\n"
                            "            </species>\n"
                            "\n"
                            "            <species id=\"B\" metaid=\"sp_2\" compartment=\"cytosol\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\"\n"
                            "                     boundaryCondition=\"false\" constant=\"false\"/>\n"
                            "            <species id=\"Ca\" metaid=\"sp_3\" compartment=\"cytosol\" initialConcentration=\"2\" hasOnlySubstanceUnits=\"false\"\n"
                            "                     boundaryCondition=\"false\" constant=\"false\"/>\n"
                            "            <species id=\"PlasmaCa\" metaid=\"sp_4\" compartment=\"extraCell\" initialConcentration=\"3\"\n"
                            "                     hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                            "            <species id=\"Enzyme\" metaid=\"sp_5\" compartment=\"cytosol\" initialConcentration=\"2\"\n"
                            "                     hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                            "\n"
                            "        </listOfSpecies>\n"
                            "        <listOfParameters>\n"
                            "            <parameter id=\"k1\" value=\"0.1\" constant=\"true\"/>\n"
                            "            <parameter id=\"k2\" value=\"0.1\" constant=\"true\"/>\n"
                            "        </listOfParameters>\n"
                            "        <listOfReactions>\n"
                            "            <reaction id=\"r1\" metaid=\"react1\" reversible=\"false\" fast=\"false\">\n"
                            "                <listOfReactants>\n"
                            "                    <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
                            "                </listOfReactants>\n"
                            "                <listOfProducts>\n"
                            "                    <speciesReference species=\"A\" stoichiometry=\"2\" constant=\"true\"/>\n"
                            "                </listOfProducts>\n"
                            "                <kineticLaw>\n"
                            "                    <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                            "                        <apply>\n"
                            "                            <times/>\n"
                            "                            <ci>k2</ci>\n"
                            "                            <ci>B</ci>\n"
                            "                        </apply>\n"
                            "                    </math>\n"
                            "                </kineticLaw>\n"
                            "            </reaction>\n"
                            "            <reaction id=\"r2\" metaid=\"react2\" reversible=\"false\" fast=\"false\">\n"
                            "                <listOfReactants>\n"
                            "                    <speciesReference species=\"Ca\" stoichiometry=\"1\" constant=\"true\"/>\n"
                            "                    <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
                            "                </listOfReactants>\n"
                            "                <listOfProducts>\n"
                            "                    <speciesReference species=\"PlasmaCa\" stoichiometry=\"1\" constant=\"true\"/>\n"
                            "                </listOfProducts>\n"
                            "                <listOfModifiers>\n"
                            "                    <modifierSpeciesReference species=\"Enzyme\">\n"
                            "                </listOfModifiers>\n"
                            "                <kineticLaw>\n"
                            "                    <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                            "                        <apply>\n"
                            "                            <times/>\n"
                            "                            <ci>k2</ci>\n"
                            "                            <ci>Ca</ci>\n"
                            "                        </apply>\n"
                            "                    </math>\n"
                            "                </kineticLaw>\n"
                            "            </reaction>\n"
                            "        </listOfReactions>\n"
                            "    </model>\n"
                            "</sbml>\n"
                            "";

}












