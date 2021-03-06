//
// Created by Ciaran on 4/4/2020.
//

#include "omexmeta/Subject.h"


namespace omexmeta {

    Subject::Subject(librdf_node *node)
            : node_(node) {
    }

    Subject::Subject(LibrdfNode node)
            : node_(node.get()) {}

    Subject Subject::fromUri(const std::string &uri) {
        return Subject(LibrdfNode::fromUriString(uri).get());
    }

    Subject Subject::fromBlank(const std::string &blank) {
        return Subject(LibrdfNode::fromBlank(blank).get());
    }

    std::string Subject::str() const {
        if (!node_) {
            throw NullPointerException("NullPointerException: Subject::str(): node_ is null");
        }
        return LibrdfNode::str(node_);
    }

    bool Subject::isSet() const {
        return node_ != nullptr;
    }

    librdf_node *Subject::getNode() const {
        return node_;
    }

    Subject Subject::fromRawPtr(librdf_node *node) {
        return Subject(node);
    }

    void Subject::free() {
        if (node_) {
            LibrdfNode::freeNode(node_);
            setNode(nullptr);
        }
        this->~Subject(); // todo needed?
    }

    void Subject::setNode(librdf_node *node) {
        node_ = node;
    }

    bool Subject::operator==(const Subject &rhs) const {
        // for now, this will change after I turn node_ back to a LibrdfNode
        return librdf_node_equals(node_, rhs.node_);
    }

    bool Subject::operator!=(const Subject &rhs) const {
        return !(rhs == *this);
    }


}