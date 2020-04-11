//
// Created by Ciaran on 4/9/2020.
//

#include <librdf.h>
#include "gtest/gtest.h"
#include "AnnotationSamples.h"
#include "librdf.h"
#include "semsim/Reader.h"


class ReaderTests : public ::testing::Test {
public:

    AnnoationSamples samples;
    librdf_world *world;
    raptor_world *raptor_world_ptr;
    librdf_storage *storage;
    librdf_model *model;

    ReaderTests() {
        world = librdf_new_world();
        librdf_world_open(world);
        raptor_world_ptr = librdf_world_get_raptor(world);
        storage = librdf_new_storage(world, "memory", "semsim_store", nullptr);
        if (!storage) {
            throw std::invalid_argument("Failed to create new storage\n");
        }
        model = librdf_new_model(world, storage, nullptr);
        if (!model) {
            throw std::invalid_argument("Failed to create model\n");
        }
    };

    void assertModelSizesAreDifferentAfterParsing(std::string annotation_string) {
        semsim::Reader reader(world, model, "rdfxml");
        int size_before = librdf_model_size(model);
        reader.fromString(annotation_string);
        int size_after = librdf_model_size(model);
        ASSERT_NE(size_before, size_after);
    }
};


TEST_F(ReaderTests, TestReaderInstantiation) {
    semsim::Reader reader(world, model, "rdfxml");
    ASSERT_TRUE(true);
}

TEST_F(ReaderTests, TestFromString) {
    assertModelSizesAreDifferentAfterParsing(samples.singular_annotation1);
}

TEST_F(ReaderTests, TestFromString2) {
    assertModelSizesAreDifferentAfterParsing(samples.singular_annotation2);
}

TEST_F(ReaderTests, TestFromString3) {
    assertModelSizesAreDifferentAfterParsing(samples.singular_annotation3);
}


TEST_F(ReaderTests, TestFromStringComposite_annotation_pe) {
    assertModelSizesAreDifferentAfterParsing(samples.composite_annotation_pe);
}

TEST_F(ReaderTests, TestFromStringComposite_annotation_pp) {
    assertModelSizesAreDifferentAfterParsing(samples.composite_annotation_pp);
}

TEST_F(ReaderTests, TestFromStringComposite_annotation_pf) {
    assertModelSizesAreDifferentAfterParsing(samples.composite_annotation_pf);
}

TEST_F(ReaderTests, TestFromStringTabular_data1) {
    assertModelSizesAreDifferentAfterParsing(samples.tabular_data1);
}


















