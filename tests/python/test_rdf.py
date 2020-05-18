import os
import site
import unittest
import ctypes as ct

test_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
proj_dir = os.path.dirname(test_dir)
src_dir = os.path.join(proj_dir, "src")
pysemsem_dir = os.path.join(src_dir, "pysemsim")

site.addsitedir(src_dir)

import pysemsim

xml = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version2/core" level="3" version="2">
  <model id="TestModelNotAnnotated">
    <listOfUnitDefinitions>
      <unitDefinition id="molar">
        <listOfUnits>
          <unit kind="mole" exponent="1" scale="1" multiplier="1"/>
          <unit kind="litre" exponent="-1" scale="1" multiplier="1"/>
        </listOfUnits>
      </unitDefinition>
    </listOfUnitDefinitions>
    <listOfCompartments>
      <compartment metaid="cytosol" id="cytosol" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species metaid="Meta00001" id="X" compartment="cytosol" initialConcentration="10" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="Y" compartment="cytosol" initialConcentration="20" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="Y" compartment="cytosol" initialConcentration="15" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
    <listOfReactions>
      <reaction id="X2Y" reversible="false">
        <listOfProducts>
          <speciesReference species="Y" constant="false"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> x </ci>
              <ci> kx2y </ci>
            </apply>
          </math>
          <listOfLocalParameters>
            <localParameter id="kx2y" value="1"/>
            <localParameter id="ky2z" value="1"/>
          </listOfLocalParameters>
        </kineticLaw>
      </reaction>
      <reaction id="y2z" reversible="false">
        <listOfProducts>
          <speciesReference species="Z" constant="false"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> y </ci>
              <ci> ky2z </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>"""

class UtilsTest(unittest.TestCase):

    def setUp(self) -> None:
        self.lib = pysemsim.Util.load_lib()

    def tearDown(self) -> None:
        pass

    def test_load_func(self):
        func_that_returns_a_str = pysemsim.Util.load_func(self.lib, "_func_that_returns_dynamic_alloc_str", [], ct.c_int64)
        self.assertTrue(callable(func_that_returns_a_str))

    def test_get_and_free_c_str(self):
        free_func = pysemsim.Util.load_func(self.lib, "free_c_char_star", [ct.c_int64], None)
        func_that_returns_a_str = pysemsim.Util.load_func(self.lib, "_func_that_returns_dynamic_alloc_str", [], ct.c_int64)
        actual = pysemsim.Util.get_and_free_c_str(func_that_returns_a_str())
        expected = "ADynamicallyAllocatedStringForTesting"
        self.assertEqual(expected, actual)


class TestRDF(unittest.TestCase):
    rdf_str = """<?xml version="1.0" encoding="utf-8"?>
    <rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
       xmlns:bqmodel="http://biomodels.net/model-qualifiers/"
       xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
       xmlns:semsim="http://www.bhi.washington.edu/semsim#"
       xml:base="file://./annotations.rdf">
      <rdf:Description rdf:about="force_0">
        <semsim:hasSinkParticipant rdf:resource="sink_0"/>
        <semsim:hasSourceParticipant rdf:resource="source_0"/>
      </rdf:Description>
      <rdf:Description rdf:about="parameter_metaid_0">
        <bqbiol:isPropertyOf rdf:resource="force_0"/>
        <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/OPB_01058"/>
      </rdf:Description>
      <rdf:Description rdf:about="sink_0">
        <semsim:hasPhysicalEntityReference rdf:resource="species_metaid_1"/>
      </rdf:Description>
      <rdf:Description rdf:about="source_0">
        <semsim:hasPhysicalEntityReference rdf:resource="species_metaid_0"/>
      </rdf:Description>
    </rdf:RDF>"""

    def test_crete_new_rdf_obj(self):
        rdf = pysemsim.RDF()
        self.assertIsInstance(rdf._obj, int)

    def test_from_string(self):
        rdf = pysemsim.RDF.from_string(self.rdf_str, "rdfxml")
        self.assertEqual(6, len(rdf))

    def test_to_string(self):
        rdf = pysemsim.RDF.from_string(self.rdf_str, "rdfxml")
        str(rdf)

    def test_get_base_uri(self):
        rdf = pysemsim.RDF.from_string(self.rdf_str, "rdfxml")
        uri = rdf.get_base_uri()
        self.assertEqual("file://./semsim_model.rdf", uri)

    def test_set_base_uri(self):
        rdf = pysemsim.RDF.from_string(self.rdf_str, "rdfxml")
        rdf.set_base_uri("ABaseUri.rdf")
        uri = rdf.get_base_uri()
        expected = "file:///mnt/d/libsemsim/tests/python/ABaseUri.rdf"
        self.assertEqual(expected, uri)

    def test_query(self):
        rdf = pysemsim.RDF.from_string(self.rdf_str, "rdfxml")
        q = """SELECT ?x ?y ?z 
        WHERE {?x ?y ?z}
        """
        expected = """x,y,z
file://./force_0,http://www.bhi.washington.edu/semsim#hasSinkParticipant,file://./sink_0
file://./force_0,http://www.bhi.washington.edu/semsim#hasSourceParticipant,file://./source_0
file://./parameter_metaid_0,http://biomodels.net/biology-qualifiers/isPropertyOf,file://./force_0
file://./parameter_metaid_0,http://biomodels.net/biology-qualifiers/isVersionOf,https://identifiers.org/opb/OPB_01058
file://./sink_0,http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference,file://./species_metaid_1
file://./source_0,http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference,file://./species_metaid_0
"""
        self.maxDiff = None
        actual = rdf.query(q, "csv")
        self.assertEqual(expected, actual)


class EditorTests(unittest.TestCase):

    def setUp(self) -> None:
        self.rdf = pysemsim.RDF()
        self.editor = self.rdf.to_editor(xml, "sbml")

    def tearDown(self) -> None:
        pass
        # self.rdf.delete()
        # self.editor.delete()

    def test_to_editor(self):
        self.assertIsInstance(self.editor, pysemsim.Editor)

    def test_singular_ann_str(self):

        singular_annotation = self.editor.new_singular_annotation()
        singular_annotation \
            .set_about("metaid4") \
            .set_predicate("bqb", "is") \
            .set_resource_uri("uniprot:PD88776")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="metaid4">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/PD88776"/>
  </rdf:Description>
</rdf:RDF>
'''
        actual = str(singular_annotation)
        self.assertEqual(expected, actual)

    def test_singular_ann_add_to_model(self):
        singular_annotation = self.editor.new_singular_annotation()
        singular_annotation \
            .set_about("metaid4") \
            .set_predicate("bqb", "is") \
            .set_resource_uri("uniprot:PD88776")
        self.editor.add_singular_annotation(singular_annotation)
        self.editor.to_rdf()
        print(self.rdf)

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="metaid4">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/PD88776"/>
  </rdf:Description>
</rdf:RDF>
'''
        actual = str(self.rdf)
        self.assertEqual(expected, actual)

    def test_physical_entity(self):

        physical_entity = self.editor.new_physical_entity()
        physical_entity \
            .set_about("metaid87") \
            .set_physical_property("opb/opb_275") \
            .set_identity("uniprot/PD72514") \
            .add_location("fma:FMA:7654")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="PhysicalEntity0000">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/PD72514"/>
    <bqbiol:isPartOf rdf:resource="https://identifiers.org/fma/FMA:7654"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid87">
    <bqbiol:isPropertyOf rdf:resource="PhysicalEntity0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb_275"/>
  </rdf:Description>
</rdf:RDF>
'''
        formats = [
            'ntriples', 'turtle', 'rdfxml-xmp', 'rdfxml-abbrev',
            'rdfxml', 'dot', 'json-triples', 'json', 'nquads',
            'html'
        ]
        for i in formats:
            actual = physical_entity.to_string(i, "./annotation.rdf")
        #todo free() : invalid pointer warning on c end
        # actual = str(physical_entity)
        """
        str(physical_entity) is the cause of the free(): invalid pointer problem.
        """
        # self.assertEqual(expected, actual)

    def test_physical_add_to_model(self):

        physical_entity = self.editor.new_physical_entity()
        physical_entity \
            .set_about("metaid87") \
            .set_physical_property("opb/opb_275") \
            .set_identity("uniprot/PD72514") \
            .add_location("fma:FMA:7654")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="PhysicalEntity0000">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/PD72514"/>
    <bqbiol:isPartOf rdf:resource="https://identifiers.org/fma/FMA:7654"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid87">
    <bqbiol:isPropertyOf rdf:resource="PhysicalEntity0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb_275"/>
  </rdf:Description>
</rdf:RDF>
'''
        self.editor.add_physical_entity(physical_entity)
        self.editor.to_rdf()
        actual = str(self.rdf)
        self.assertEqual(expected, actual)

    def test_physical_process(self):

        physical_process = self.editor.new_physical_process()
        physical_process \
            .set_about("metaid87") \
            .set_physical_property("opb/opb_275") \
            .add_source("metaid2", 1.0, "physicalEntity4") \
            .add_sink("metaid3", 1.0, "PhysicalEntity7") \
            .add_mediator("metaid002", 1.0, "PhysicalEntity9")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:semsim="http://www.bhi.washington.edu/semsim#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="PhysicalProcess0000">
    <semsim:hasMediatorParticipant rdf:resource="metaid002"/>
    <semsim:hasSinkParticipant rdf:resource="metaid3"/>
    <semsim:hasSourceParticipant rdf:resource="metaid2"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid002">
    <semsim:hasPhysicalEntityReference rdf:resource="PhysicalEntity9"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid2">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="physicalEntity4"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid3">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="PhysicalEntity7"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid87">
    <bqbiol:isPropertyOf rdf:resource="PhysicalProcess0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb_275"/>
  </rdf:Description>
</rdf:RDF>
'''
        actual = str(physical_process)
        self.assertEqual(expected, actual)

    def test_physical_force(self):

        physical_force = self.editor.new_physical_force()
        physical_force \
            .set_about("metaid87") \
            .set_physical_property("opb/opb_275") \
            .add_source("metaid2", 1.0, "physicalEntity4") \
            .add_sink("metaid3", 1.0, "PhysicalEntity7")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:semsim="http://www.bhi.washington.edu/semsim#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="PhysicalForce0000">
    <semsim:hasSinkParticipant rdf:resource="metaid3"/>
    <semsim:hasSourceParticipant rdf:resource="metaid2"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid2">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="physicalEntity4"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid3">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="PhysicalEntity7"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid87">
    <bqbiol:isPropertyOf rdf:resource="PhysicalForce0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb_275"/>
  </rdf:Description>
</rdf:RDF>
'''
        actual = str(physical_force)
        # self.assertEqual(expected, actual)

    def test_physical_force_Add_to_model(self):
        physical_force = self.editor.new_physical_force()

        physical_force \
            .set_about("metaid87") \
            .set_physical_property("opb/opb_275") \
            .add_source("metaid2", 1.0, "physicalEntity4") \
            .add_sink("metaid3", 1.0, "PhysicalEntity7")

        expected = '''<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:semsim="http://www.bhi.washington.edu/semsim#"
   xml:base="file://./Annotation.rdf">
  <rdf:Description rdf:about="PhysicalForce0000">
    <semsim:hasSinkParticipant rdf:resource="metaid3"/>
    <semsim:hasSourceParticipant rdf:resource="metaid2"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid2">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="physicalEntity4"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid3">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/2001/XMLSchema#double">5.26354e-315</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="PhysicalEntity7"/>
  </rdf:Description>
  <rdf:Description rdf:about="metaid87">
    <bqbiol:isPropertyOf rdf:resource="PhysicalForce0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb_275"/>
  </rdf:Description>
</rdf:RDF>
'''

        self.editor.add_physical_force(physical_force)
        self.editor.to_rdf()
        self.rdf.to_string("rdfxml-abbrev", base_uri="./Annotation.rdf")
        # actual = str(rdf)
        # self.assertEqual(expected, actual)

    def test(self):
        rdf = pysemsim.RDF()
        editor = rdf.to_editor(xml, "sbml")

        singular_annotation1 = editor.new_singular_annotation()
        singular_annotation1 \
            .set_about("metaid4") \
            .set_predicate("bqb", "is") \
            .set_resource_uri("uniprot:PD88776")

        singular_annotation2 = editor.new_singular_annotation()
        singular_annotation2 \
            .set_about("metaid4") \
            .set_predicate("bqb", "isPartOf") \
            .set_resource_uri("FMA:fma:09837")

        editor.add_singular_annotation(singular_annotation1)
        editor.add_singular_annotation(singular_annotation2)
        editor.to_rdf()

        print(rdf)
        print("delet")
        editor.delete()
        print("delet")
        rdf.delete()

        rdf2 = pysemsim.RDF()
        editor2 = rdf.to_editor(xml, "sbml")

        singular_annotation3 = editor2.new_singular_annotation()
        singular_annotation3 \
            .set_about("metaid5") \
            .set_predicate("bqb", "is") \
            .set_resource_uri("uniprot:PD88776")

        singular_annotation4 = editor2.new_singular_annotation()
        singular_annotation4 \
            .set_about("metaid3") \
            .set_predicate("bqb", "isPartOf") \
            .set_resource_uri("FMA:fma:09837")

        editor2.add_singular_annotation(singular_annotation3)
        editor2.add_singular_annotation(singular_annotation4)
        editor2.to_rdf()

        print(rdf2)


if __name__ == "__main__":
    unittest.main()