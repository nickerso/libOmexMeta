//
// Created by Ciaran on 4/15/2020.
//
#include "gtest/gtest.h"
#include "SBMLFactory.h"
#include "omexmeta/Editor.h"
#include "omexmeta/SemsimXmlAssistant.h"
#include "omexmeta/PhysicalEntity.h"
#include "omexmeta/PhysicalProcess.h"
#include "omexmeta/PhysicalForce.h"
#include "omexmeta/RDF.h"

using namespace omexmeta;

class EditorTests : public ::testing::Test {

public:

    LibrdfStorage storage;
    LibrdfModel model;

    EditorTests() {
        model = LibrdfModel(storage.get());
    };

    ~EditorTests() override {
        model.freeModel();
        storage.freeStorage();
    }

};

TEST_F(EditorTests, TestMetaIds) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    const auto &metaids = editor.getMetaids();
    std::vector<std::string> expected = {"#OmexMetaId0000", "#OmexMetaId0001", "#OmexMetaId0002",
                                         "#cytosol", "#Meta00001", "#OmexMetaId0003", "#OmexMetaId0004",
                                         "#OmexMetaId0005", "#OmexMetaId0006", "#OmexMetaId0007",
                                         "#OmexMetaId0008", "#OmexMetaId0009", "#OmexMetaId0010"};
    ASSERT_EQ(expected, metaids);
}

TEST_F(EditorTests, TestRepositoryName1) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    std::string expected = "http://myOmexLibrary.org/";
    std::string actual = editor.getOmexRepository();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestepositoryName2) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    std::string expected = "http://myCustomOmexLibrary.org/";
    editor.setOmexRepository(expected);
    std::string actual = editor.getOmexRepository();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestArchiveName) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    std::string expected = "http://myOmexLibrary.org/myomex.omex";
    editor.setArchiveName("myomex");
    std::string actual = editor.getArchiveName();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestArchiveName2) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    std::string expected = "http://myOmexLibrary.org/myomex.omex";
    editor.setArchiveName("myomex.omex");
    std::string actual = editor.getArchiveName();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestArchiveName3) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    std::string expected = "http://myOmexLibrary.org/myomex.omex";
    editor.setArchiveName("http://myOmexLibrary.org/myomex.omex");
    std::string actual = editor.getArchiveName();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}


TEST_F(EditorTests, TestSetModelName) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    editor.setArchiveName("MyOmexArchive");
    editor.setModelName("smad.sbml");

    std::string expected = "http://myOmexLibrary.org/MyOmexArchive.omex/smad.sbml";
    ASSERT_STREQ(expected.c_str(), editor.getModelName().c_str());
}

TEST_F(EditorTests, TestSetLocalName) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    editor.setArchiveName("MyOmexArchive");
    editor.setModelName("smad.sbml");
    editor.setLocalName("smad");

    std::string expected = "http://myOmexLibrary.org/MyOmexArchive.omex/smad.rdf";
    ASSERT_STREQ(expected.c_str(), editor.getLocalName().c_str());
}

TEST_F(EditorTests, TestSetLocalNam) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    editor.setArchiveName("MyOmexArchive");
    editor.setModelName("smad.sbml");
    editor.setLocalName("smad.rdf");

    std::string expected = "http://myOmexLibrary.org/MyOmexArchive.omex/smad.rdf";
    ASSERT_STREQ(expected.c_str(), editor.getLocalName().c_str());
}


TEST_F(EditorTests, TestAddAnnotation) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    PredicatePtr predicatePtr = std::make_shared<Predicate>(
            BiomodelsBiologyQualifier("is"));
    editor.addSingleAnnotation(
            Subject(LibrdfNode::fromUriString("#OmexMetaId0004")),
            predicatePtr,
            Resource(LibrdfNode::fromUriString("uniprot:P0DP23"))
    );
    ASSERT_EQ(1, editor.size());
//    triples.freeTriples();
}

TEST_F(EditorTests, TestAddSingleAnnotationToEditor) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    Triple triple(LibrdfNode::fromUriString("#OmexMetaId0009").get(),
                  BiomodelsBiologyQualifier("is").getNode(),
                  Resource(LibrdfNode::fromUriString("uniprot:P0DP23")).getNode());
    editor.addSingleAnnotation(triple);
    int expected = 1;
    int actual = editor.size();
    ASSERT_EQ(expected, actual);
    triple.freeStatement();
}

TEST_F(EditorTests, TestEditorCreateUriRelativeToLocalUri) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    editor.setArchiveName("MyOmexArchive");
    editor.setModelName("mymodel.sbml");
    editor.setLocalName("mymodel.rdf");
    LibrdfNode node = editor.createNodeWithLocalUri("#OmexMetaId0009");
    std::string actual = node.str();
    std::string expected = "http://myOmexLibrary.org/MyOmexArchive.omex/mymodel.rdf#OmexMetaId0009";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    node.freeNode();
}

TEST_F(EditorTests, TestAddSingleAnnotationToRDF1) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    Subject subject = Subject(editor.createNodeWithLocalUri("#OmexMetaId0009"));
    BiomodelsBiologyQualifier predicate("is");
    Resource resource = Resource(LibrdfNode::fromUriString("uniprot:P0DP23"));
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    editor.addSingleAnnotation(triple);

    std::string actual = rdf.toString("rdfxml", "MyModel");
    std::cout << actual << std::endl;
    std::string expected = "";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();
}

TEST_F(EditorTests, TestAddSingleAnnotationToRDF2) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    editor.addSingleAnnotation(
            Subject(LibrdfNode::fromUriString("#OmexMetaId0008")),
            std::make_shared<Predicate>(BiomodelsBiologyQualifier("isDescribedBy")),
            Resource(LibrdfNode::fromUriString("pubmed:12991237"))
    );

    std::string actual = rdf.toString("turtle", "file://./annotations.rdf");
    std::string expected = "@base <file://./annotations.rdf> .\n"
                           "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                           "\n"
                           "<#OmexMetaId0008>\n"
                           "    bqbiol:isDescribedBy <https://identifiers.org/pubmed/12991237> .\n"
                           "\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestAddSingleAnnotationToRDF3) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    editor.addSingleAnnotation(
            Subject(LibrdfNode::fromUriString("#OmexMetaId0008")),
            std::make_unique<Predicate>(BiomodelsBiologyQualifier("isDescribedBy")),
            Resource(LibrdfNode::fromUriString("pubmed:12991237"))
    );

    std::string actual = rdf.toString("rdfxml", "file://./annotations.rdf");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xml:base=\"/mnt/d/libOmexMeta/cmake-build-debug-wsl-ubuntu1804-gcc101/bin/\">\n"
                           "  <rdf:Description rdf:about=\"http://myOmexLibrary.org/MyOmex.omex/mymodel.rdf#OmexMetaId0009\">\n"
                           "    <bqbiol:is rdf:resource=\"https://identifiers.org/uniprot/P0DP23\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestToRDFSingularAnnotationWithLiteral) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    editor.
            addSingleAnnotation(
            Subject(LibrdfNode::fromUriString("#OmexMetaId0008")),
            std::make_unique<Predicate>(DCTerm("description")),
            Resource(LibrdfNode::fromLiteral("Cardiomyocyte cytosolic ATP concentration"))
    );

    std::string actual = rdf.toString("rdfxml", "file://./annotations.rdf");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xml:base=\"file://./annotations.rdf\">\n"
                           "  <rdf:Description rdf:about=\"#OmexMetaId0008\">\n"
                           "    <dcterms:Description rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#string\">Cardiomyocyte cytosolic ATP concentration</dcterms:Description>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    std::cout << actual <<
              std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestSingularAnnotWithBuilderPattern) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    SingularAnnotation singularAnnotation;
    singularAnnotation
            .setAbout("#OmexMetaId0001")
            .setPredicate("bqbiol", "isVersionOf")
            .setResourceUri("uniprot:PD02635");

    editor.addSingleAnnotation(singularAnnotation);

    std::string actual = rdf.toString("rdfxml", "MyModel.rdf");
    std::cout << actual << std::endl;
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xml:base=\"file://MyModel.rdf\">\n"
                           "  <rdf:Description rdf:about=\"#OmexMetaId0001\">\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/uniprot/PD02635\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    singularAnnotation.freeStatement();

}


TEST_F(EditorTests, TestAddPhysicalEntityToEditor) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    PhysicalProperty ppr("metaid", "OPB:OPB_00154");
    Resource r(LibrdfNode::fromUriString("fma:FMA:9670")); // is smad3
    std::vector<Resource> resources;
    resources.emplace_back(std::move(LibrdfNode::fromUriString("fma/FMA:9697")));
    PhysicalEntity physicalEntity = PhysicalEntity(
            rdf.getModel(), ppr, r, resources
    );
//    std::shared_ptr<PhysicalEntity> ptr = std::make_shared<PhysicalEntity>(physicalEntity);
    editor.addCompositeAnnotation(&physicalEntity);
    int expected = 4;
    int actual = editor.size();
    ASSERT_EQ(expected, actual);

}


TEST_F(EditorTests, TestAddAnnotationCompositeTypePhysicalProcess) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalProcess process = PhysicalProcess(
            model.get(),
            PhysicalProperty("#MetaId004", "OPB:OPB1234"),
            std::vector<SourceParticipant>(
                    {SourceParticipant(model.get(),
                                       1.0,
                                       "#PhysicalEntityReference1"
                    )}
            ),
            std::vector<SinkParticipant>(
                    {SinkParticipant(model.get(),
                                     1.0,
                                     "#PhysicalEntityReference2"
                    )}
            ),
            std::vector<MediatorParticipant>(
                    {MediatorParticipant(model.get(),
                                         "#PhysicalEntityReference3"
                    )}
            )
    );

    editor.addCompositeAnnotation(&process);

    std::string actual = rdf.toString("rdfxml-abbrev", "file://./annotations.rdf");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xmlns:semsim=\"http://www.bhi.washington.edu/semsim#\"\n"
                           "   xml:base=\"file://./annotations.rdf\">\n"
                           "  <rdf:Description rdf:about=\"#MediatorParticipant0000\">\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"#PhysicalEntityReference3\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#MetaId004\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"#PhysicalProcess0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/OPB/OPB1234\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#PhysicalProcess0000\">\n"
                           "    <semsim:hasMediatorParticipant rdf:resource=\"#MediatorParticipant0000\"/>\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"#SinkParticipant0000\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"#SourceParticipant0000\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#SinkParticipant0000\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"#PhysicalEntityReference2\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#SourceParticipant0000\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"#PhysicalEntityReference1\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n"
                           "";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST_F(EditorTests, TestAddAnnotationCompositeTypePhysicalForce) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalForce force = PhysicalForce(
            model.get(),
            PhysicalProperty("#metaid", "OPB:OPB1234"),
            std::vector<SourceParticipant>(
                    {SourceParticipant(model.get(),
                                       1.0,
                                       "#PhysicalEntityReference1"
                    )}
            ),
            std::vector<SinkParticipant>(
                    {SinkParticipant(model.get(),
                                     1.0,
                                     "#PhysicalEntityReference2"
                    )}
            )
    );

    editor.addCompositeAnnotation(&force);


    std::string actual = rdf.toString("rdfxml-abbrev", "file://./annotations.rdf");
    std::cout << actual <<
              std::endl;
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xmlns:semsim=\"http://www.bhi.washington.edu/semsim#\"\n"
                           "   xml:base=\"file://./annotations.rdf\">\n"
                           "  <rdf:Description rdf:about=\"#PhysicalForce0000\">\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"#SinkParticipant0000\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"#SourceParticipant0000\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#SinkParticipant0000\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"#PhysicalEntityReference2\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#SourceParticipant0000\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"#PhysicalEntityReference1\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"#metaid\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"#PhysicalForce0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/OPB/OPB1234\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}


TEST_F(EditorTests, TestSingularAnnotationBuilder) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);


    SingularAnnotation singularAnnotation;
    singularAnnotation
            .setAbout("#OmexMetaId0000")
            .setPredicate("bqbiol", "is")
            .setResourceLiteral("resource");

    editor.addSingleAnnotation(singularAnnotation);

    int expected = 1;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
    singularAnnotation.freeStatement();
}


TEST_F(EditorTests, TestPhysicalEntityBuilder) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");

    editor.addPhysicalEntity(physicalEntity);


    int expected = 4;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}


TEST_F(EditorTests, TestPhysicalForceBuilder) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalForce physicalForce = editor.createPhysicalForce();
    physicalForce
            .setPhysicalProperty("#OmexMetaId0000", "OPB:OPB1234")
            .addSource(1.0, "PhysicalEntity1")
            .addSink(1.0, "PhysicalEntity2");

    editor.addPhysicalForce(physicalForce);


    int expected = 8;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}

TEST_F(EditorTests, TestPhysicalProcessBuilder) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalProcess physicalProcess = editor.createPhysicalProcess();
    physicalProcess
            .setPhysicalProperty("#OmexMetaId0000", "OPB:OPB1234")
            .addSource(1.0, "PhysicalEntity1")
            .addSink(1.0, "PhysicalEntity2")
            .addMediator(1.0, "PhysicalEntity3");

    editor.addPhysicalProcess(physicalProcess);

    int expected = 10;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}


TEST_F(EditorTests, TestRemoveSingularAnnotation) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    SingularAnnotation singularAnnotation;
    singularAnnotation
            .setAbout("#OmexMetaId0000")
            .setPredicate("bqbiol", "is")
            .setResourceLiteral("resource");

    editor.addSingleAnnotation(singularAnnotation);

    ASSERT_EQ(1, rdf.size());

    editor.removeSingleAnnotation(singularAnnotation);
    int expected = 0;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
    singularAnnotation.freeStatement();
}

TEST_F(EditorTests, TestRemovePhysicalForce) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalForce physicalForce = editor.createPhysicalForce();
    physicalForce
            .setPhysicalProperty("#OmexMetaId0002", "OPB:OPB1234")
            .addSource(1.0, "PhysicalEntity1")
            .addSink(1.0, "PhysicalEntity2");
    editor.addPhysicalForce(physicalForce);
    ASSERT_EQ(8, rdf.size());
    editor.removePhysicalForce(physicalForce);
    int expected = 0;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}


TEST_F(EditorTests, TestRemovePhysicalProcess) {
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);

    PhysicalProcess physicalProcess = editor.createPhysicalProcess();
    physicalProcess
            .setPhysicalProperty("#OmexMetaId0004", "OPB:OPB1234")
            .addSource(1.0, "PhysicalEntity1")
            .addMediator(1.0, "PhysicalEntity1")
            .addSink(1.0, "PhysicalEntity2");
    editor.addPhysicalProcess(physicalProcess);
    ASSERT_EQ(10, rdf.size());
    editor.removePhysicalProcess(physicalProcess);
    int expected = 0;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}


/*****************************************************************
 * Test PhysicalEntity memory accountability
 */
class EditorTestsPhysicalEntityMemory : public ::testing::Test {
public:

    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();

    EditorTestsPhysicalEntityMemory() {
        physicalEntity
                .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
                .setIdentity("uniprot:PD12345")
                .addLocation("fma:fma:1234");

    };
};

TEST_F(EditorTestsPhysicalEntityMemory, TestPhysicalEntityAdds4TriplesToModel) {

    editor.addPhysicalEntity(physicalEntity);
    ASSERT_EQ(4, rdf.size());
}

TEST_F(EditorTestsPhysicalEntityMemory, TestUnpackTriplesAndAccountForStatements) {
    Triples triples = physicalEntity.toTriples();

    // unpack triples
    Triple triple4 = triples.pop();
    Triple triple3 = triples.pop();
    Triple triple2 = triples.pop();
    Triple triple1 = triples.pop();

    // make sure we've emptied the triples object
    ASSERT_EQ(0, triples.size());

    // check that all statements have 1 usage
    ASSERT_EQ(1, triple1.getStatement()->usage);
    ASSERT_EQ(1, triple2.getStatement()->usage);
    ASSERT_EQ(1, triple3.getStatement()->usage);
    ASSERT_EQ(1, triple4.getStatement()->usage);

    triple4.freeStatement();
    triple3.freeStatement();
    triple2.freeStatement();
    triple1.freeStatement();

}

TEST_F(EditorTestsPhysicalEntityMemory, TestUnpackTriplesAndAccountForTerms) {
    Triples triples = physicalEntity.toTriples();

    // unpack triples
    Triple triple4 = triples.pop();
    Triple triple3 = triples.pop();
    Triple triple2 = triples.pop();
    Triple triple1 = triples.pop();

    // make sure we've emptied the triples object
    ASSERT_EQ(0, triples.size());

    ASSERT_EQ(1, triple1.getSubject()->usage);
    ASSERT_EQ(1, triple2.getSubject()->usage);
    ASSERT_EQ(1, triple3.getSubject()->usage);
    ASSERT_EQ(1, triple4.getSubject()->usage);

    ASSERT_EQ(1, triple1.getPredicate()->usage);
    ASSERT_EQ(1, triple2.getPredicate()->usage);
    ASSERT_EQ(1, triple3.getPredicate()->usage);
    ASSERT_EQ(1, triple4.getPredicate()->usage);

    ASSERT_EQ(1, triple1.getResource()->usage);
    ASSERT_EQ(1, triple2.getResource()->usage);
    ASSERT_EQ(1, triple3.getResource()->usage);
    ASSERT_EQ(1, triple4.getResource()->usage);

    triple4.freeStatement();
    triple3.freeStatement();
    triple2.freeStatement();
    triple1.freeStatement();

}



/**********************************************
 * Tests for deleting PhysicalEntity
 */

/*
 * Note: This was way harder than it should have been because I neglected to
 * remember that the toTriples() method autogenerates some subject portions of
 * some triples. Therefore calling it twice would generate a new metaid and
 * it wouldn't be equal to the originally generated annotation.
 *
 * Took me a while to figure this out, hence why there are loads of remove* tests.
 */
class EditorTestsDeletePhysicalEntity : public ::testing::Test {
public:
    RDF rdf;
    Editor editor = rdf.toEditor(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML);


    EditorTestsDeletePhysicalEntity() = default;

};

TEST_F(EditorTestsDeletePhysicalEntity, TestRDFSizeBeforeRemovingAndNoMemoryLeaks) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    editor.addPhysicalEntity(physicalEntity);
    ASSERT_EQ(4, rdf.size());
    Triples triples = physicalEntity.toTriples();
    ASSERT_EQ(4, triples.size());
    triples.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTriple1) {
    Triple triple(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTriple2) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    Triple triple(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0001")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTriple3) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    Triple triple(
            LibrdfNode::fromUriString("#Meta00001"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTriple4) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    Triple triple(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTripleWithFreeInMiddle) {
    Triple triple(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    triple.freeTriple();

    Triple triple2(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(0, rdf.size());
    triple2.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveSingleTripleFromTriples) {
    Triple triple(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples(1);
    triples.move_back(triple);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotation(it);
    }

    // check rdf size
    ASSERT_EQ(1, rdf.size());

    // free triples
    triples.freeTriples();

    // get the Triples again. Same content, different instance
    Triple triple2(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples2(1);
    triples2.move_back(triple2);

    // use it to remove the triple
    for (auto &it : triples2) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples2.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesDifferentContent) {
    // Here these triples have different content. Next test they have same content.
    Triple triple1(
            LibrdfNode::fromUriString("cytosol1"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf1"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12341")
    );
    Triple triple2(
            LibrdfNode::fromUriString("cytosol2"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf2"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12342")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    // check rdf size
    ASSERT_EQ(2, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesSameSubject) {
    // Here these triples have different content. Next test they have same content.
    Triple triple1(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf1"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12341")
    );
    Triple triple2(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf2"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12342")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    // check rdf size
    ASSERT_EQ(2, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesSameSubjectAndPredicate) {
    // Here these triples have different content. Next test they have same content.
    Triple triple1(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12341")
    );
    Triple triple2(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:12342")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    // check rdf size
    ASSERT_EQ(2, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesSameSubjectAndResource) {
    // Here these triples have different content. Next test they have same content.
    Triple triple1(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf1"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple2(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf2"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    // check rdf size
    ASSERT_EQ(2, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesSamePredicateAndResource) {
    // Here these triples have different content. Next test they have same content.
    Triple triple1(
            LibrdfNode::fromUriString("cytosol1"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple2(
            LibrdfNode::fromUriString("cytosol2"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    // check rdf size
    ASSERT_EQ(2, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesSameEverything) {
    /*
     * When trying to add a duplicate triple to the model,
     * it seems that raptor catches it and does not add the triple again.
     */
    Triple triple1(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );

    Triple triple2(
            LibrdfNode::fromUriString("cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotationNoValidation(it);
    }

    std::cout << rdf.toString() << std::endl;

    // check rdf size (using gtest here breaks the test? :/)
    // this is 1 because these two triples are duplicates.
    // Raptor catches duplicates
    ASSERT_EQ(1, rdf.size());

    // use it to remove the triple
    for (auto &it : triples) {
        editor.removeSingleAnnotation(it);
    }
    std::cout << rdf.toString() << std::endl;
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples.freeTriples();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveTwoTripleObjsFromTriplesWithFreeInMiddle) {
    Triple triple1(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple2(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples(2);
    triples.move_back(triple1);
    triples.move_back(triple2);

    // add to the model
    for (auto &it : triples) {
        editor.addSingleAnnotation(it);
    }

    // check rdf size
    // These are duplicates, so only 1 will be added to the raptor model
    ASSERT_EQ(1, rdf.size());

    // free triples
    triples.freeTriples();

    Triple triple3(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#cytosol"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    // put triple in Triples
    Triples triples2(2);
    triples2.move_back(triple3);
    triples2.move_back(triple4);

    // use it to remove the triple
    for (auto &it : triples2) {
        editor.removeSingleAnnotation(it);
    }
    // should be no triples left now in rdf
    ASSERT_EQ(0, rdf.size());

    // free
    triples2.freeTriples();

}


TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveDoubleTriple1And2Sequential) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    Triple triple(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();

    Triple triple2(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0001")
    );
    editor.addSingleAnnotation(triple2);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(0, rdf.size());
    triple2.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemoveDoubleTriple1And2SequentialAndFreeOnlyAtEnd) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    Triple triple(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );
    editor.addSingleAnnotation(triple);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(0, rdf.size());

    Triple triple2(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0001")
    );
    editor.addSingleAnnotation(triple2);
    ASSERT_EQ(1, rdf.size());
    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(0, rdf.size());
    triple.freeTriple();
    triple2.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemoveTripleFromAPhysicalPropertyWithoutDeletingItInMiddle) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    PhysicalProperty property("#OmexMetaId0001", "https://identifiers.org/opb/opb_1234");
    Triples triples = property.toTriples("Entity1234");
    std::cout << triples.str() << std::endl;
    for (auto &it: triples) {
        editor.addSingleAnnotation(it);
    }
    ASSERT_EQ(2, rdf.size());
    for (auto &it: triples) {
        editor.removeSingleAnnotation(it);
    }
    ASSERT_EQ(0, rdf.size());
    triples.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemoveTripleFromAPhysicalPropertyWithFreeInMiddle) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    PhysicalProperty property("#OmexMetaId0001", "https://identifiers.org/opb/opb_1234");
    Triples triples = property.toTriples("Entity1234");
    std::cout << rdf.toString() << std::endl;
    for (auto &it: triples) {
        editor.addSingleAnnotation(it);
    }
    std::cout << rdf.toString() << std::endl;
    ASSERT_EQ(2, rdf.size());
    triples.freeTriples();

    Triples triples2 = property.toTriples("Entity1234");
    std::cout << triples2.size() << std::endl;

    for (auto &it: triples2) {
        std::cout << it.str() << std::endl;
        editor.removeSingleAnnotation(it);
    }
    std::cout << rdf.toString() << std::endl;
    ASSERT_EQ(0, rdf.size());
    triples2.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemoveTripleFromAPropertyOfPhysicalEntity) {
    /* Take and delete triple number 1
 * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
 * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
 * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
 * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
 */
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    std::cout << triples.str() << std::endl;
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
    }
    ASSERT_EQ(4, rdf.size());
    for (auto &it: triples) {
        editor.removeSingleAnnotation(it);
    }
    ASSERT_EQ(0, rdf.size());
    triples.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity,
       TestCreateAddAndRemoveTripleFromAPropertyOfPhysicalEntityWithNamespaceHandling) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    std::cout << triples.str() << std::endl;
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    for (auto &it: triples) {
        editor.removeSingleAnnotation(it);
    }
    ASSERT_EQ(0, rdf.size());
    triples.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemoveTripleFromAPropertyOfPhysicalEntityWithFreeInMiddle) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    std::cout << rdf.toString() << std::endl;
    Triples triples2 = physicalEntity.toTriples();
    for (auto &it: triples2) {
        editor.removeSingleAnnotation(it);
    }
    std::cout << rdf.toString() << std::endl;

    ASSERT_EQ(0, rdf.size());
    triples2.freeTriples();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemovePhysicalEntityUsingSingleTripleObjsRemove1) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    Triple triple1(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    Triple triple2(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple3(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("#PhysicalEntity0000")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );

    editor.removeSingleAnnotation(triple1);
    ASSERT_EQ(3, rdf.size());

    triple1.freeTriple();
    triple2.freeTriple();
    triple3.freeTriple();
    triple4.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemovePhysicalEntityUsingSingleTripleObjsRemove2) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    Triple triple1(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    Triple triple2(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple3(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0000")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );

    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(3, rdf.size());

    triple1.freeTriple();
    triple2.freeTriple();
    triple3.freeTriple();
    triple4.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemovePhysicalEntityUsingSingleTripleObjsRemove3) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    Triple triple1(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    Triple triple2(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple3(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("#PhysicalEntity0000")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );

    editor.removeSingleAnnotation(triple3);
    ASSERT_EQ(3, rdf.size());

    triple1.freeTriple();
    triple2.freeTriple();
    triple3.freeTriple();
    triple4.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemovePhysicalEntityUsingSingleTripleObjsRemove4) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    Triple triple1(
            LibrdfNode::fromUriString("PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    Triple triple2(
            LibrdfNode::fromUriString("PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple3(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0000")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );

    editor.removeSingleAnnotation(triple4);
    ASSERT_EQ(3, rdf.size());

    triple1.freeTriple();
    triple2.freeTriple();
    triple3.freeTriple();
    triple4.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestCreateAddAndRemovePhysicalEntityUsingSingleTripleObjsRemove1And2) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    Triples triples = physicalEntity.toTriples();
    for (auto &it: triples) {
        editor.addSingleAnnotationNoValidation(it);
        editor.addNamespaceFromAnnotation(it.getPredicateStr());
    }
    ASSERT_EQ(4, rdf.size());
    triples.freeTriples();

    Triple triple1(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/is"),
            LibrdfNode::fromUriString("https://identifiers.org/uniprot/PD12345")
    );
    Triple triple2(
            LibrdfNode::fromUriString("#PhysicalEntity0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPartOf"),
            LibrdfNode::fromUriString("https://identifiers.org/fma/fma:1234")
    );
    Triple triple3(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isPropertyOf"),
            LibrdfNode::fromUriString("PhysicalEntity0000")
    );
    Triple triple4(
            LibrdfNode::fromUriString("#OmexMetaId0000"),
            LibrdfNode::fromUriString("http://biomodels.net/biology-qualifiers/isVersionOf"),
            LibrdfNode::fromUriString("https://identifiers.org/opb/opb_1234")
    );

    editor.removeSingleAnnotation(triple1);
    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(2, rdf.size());

    triple1.freeTriple();
    triple2.freeTriple();
    triple3.freeTriple();
    triple4.freeTriple();
}


TEST_F(EditorTestsDeletePhysicalEntity, TestAddAndRemovePhysicalEntity) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    editor.addPhysicalEntity(physicalEntity);
    ASSERT_EQ(4, rdf.size());
    editor.removePhysicalEntity(physicalEntity);
    ASSERT_EQ(0, rdf.size());


}


TEST_F(EditorTestsDeletePhysicalEntity, TestDeleteFirstTriple) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    editor.addPhysicalEntity(physicalEntity);

    /* Take and delete triple number 1
     * 1) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isVersionOf> <https://identifiers.org/opb/opb_1234> .
     * 2) <#OmexMetaId0000> <http://biomodels.net/biology-qualifiers/isPropertyOf> <PhysicalEntity0001> .
     * 3) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/is> <https://identifiers.org/uniprot/PD12345> .
     * 4) <PhysicalEntity0001> <http://biomodels.net/biology-qualifiers/isPartOf> <https://identifiers.org/fma/fma:1234> .
     */

    Triples triples = physicalEntity.toTriples();
    std::cout << triples.str("ntriples", "base") << std::endl;
    Triple triple = triples.pop_front();

    editor.removeSingleAnnotation(triple);
    ASSERT_EQ(3, rdf.size());

    triples.freeTriples();
    triple.freeTriple();
}

TEST_F(EditorTestsDeletePhysicalEntity, TestDeleteOneByOne) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    editor.addPhysicalEntity(physicalEntity);

    Triples triples = physicalEntity.toTriples();

    ASSERT_EQ(4, rdf.size());
    std::cout << "trip: " << triples.str("rdfxml-abbrev", "base") << std::endl;
    std::cout << "rdf" << rdf.toString("rdfxml-abbrev", "base") << std::endl;

    Triple triple4 = triples.pop_front();
    std::cout << triple4.str("ntriples", "base") << std::endl;
    editor.removeSingleAnnotation(triple4);
    int s = rdf.size();
    ASSERT_EQ(3, rdf.size());
    triple4.freeTriple();

    Triple triple3 = triples.pop_front();
    std::cout << triple3.str("ntriples", "base") << std::endl;
    editor.removeSingleAnnotation(triple3);
    ASSERT_EQ(2, rdf.size());
    triple3.freeTriple();

    Triple triple2 = triples.pop_front();
    std::cout << triple2.str("ntriples", "base") << std::endl;
    editor.removeSingleAnnotation(triple2);
    ASSERT_EQ(1, rdf.size());
    triple2.freeTriple();

    Triple triple1 = triples.pop_front();
    std::cout << triple1.str("ntriples", "base") << std::endl;
    editor.removeSingleAnnotation(triple1);
    ASSERT_EQ(0, rdf.size());
    triple1.freeTriple();

}

TEST_F(EditorTestsDeletePhysicalEntity, TestRemovePhysicalEntityInLoop) {
    PhysicalEntity physicalEntity = editor.createPhysicalEntity();
    physicalEntity
            .setPhysicalProperty("#OmexMetaId0000", "opb:opb_1234")
            .setIdentity("uniprot:PD12345")
            .addLocation("fma:fma:1234");
    editor.addPhysicalEntity(physicalEntity);
    ASSERT_EQ(4, rdf.size());
    editor.removePhysicalEntity(physicalEntity);
    ASSERT_EQ(0, rdf.size());
}

















