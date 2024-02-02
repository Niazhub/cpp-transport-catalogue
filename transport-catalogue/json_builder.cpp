#include "json_builder.h"

namespace json {
    BaseContext Builder::Value(Node::Value value) {
        if (is_end) {
            throw std::logic_error("Incorrect use of a method 'Value' in context");
        }
        if (!nodes_stack_.empty()) {
            if (current_key) {
                SetDict(value);
                current_key.reset();
            }
            else if (nodes_stack_.back()->IsArray()) {
                SetArray(value);
            }
            else if (nodes_stack_.back()->IsDict()) {
                throw std::logic_error("Incorrect use of a method 'Value' in context");
            }
        }
        else {
            SetValue(value);
        }
        return BaseContext(*this);
    }

    KeyItemContext Builder::Key(std::string key) {
        if (is_end || current_key.has_value() || nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Incorrect use of a method 'Key' in context");
        }
        current_key = std::move(key);
        return KeyItemContext(*this);
    }

    DictItemContext Builder::StartDict() {
        if (is_end) {
            throw std::logic_error("Incorrect use of a method 'StartDict' in context");
        }
        if (nodes_stack_.empty()) {
            root_ = Dict{};
            nodes_stack_.push_back(&root_);
        }
        else {
            auto new_root = Dict{};
            if (current_key) {
                nodes_stack_.back()->AsDict()[*current_key] = std::move(new_root);
                current_key.reset();
                nodes_stack_.emplace_back(&nodes_stack_.back()->AsDict()[*current_key]);
            }
            else {
                nodes_stack_.back()->AsArray().emplace_back(std::move(new_root));
                nodes_stack_.emplace_back(&nodes_stack_.back()->AsArray().back());
            }
        }
        return DictItemContext(*this);
    }

    ArrayItemContext Builder::StartArray() {
        if (is_end) {
            throw std::logic_error("Incorrect use of a method 'StartArray' in context");
        }
        if (nodes_stack_.empty()) {
            root_ = Array{};
            nodes_stack_.push_back(&root_);
        }
        else {
            if (current_key) {
                root_.AsDict()[*current_key] = Array{};
                nodes_stack_.emplace_back(&root_.AsDict()[*current_key]);
                current_key.reset();
            }
            else {
                nodes_stack_.back()->AsArray().push_back(std::move(root_));
                nodes_stack_.emplace_back(&nodes_stack_.back()->AsArray().back());
            }
        }
        return ArrayItemContext(*this);
    }

    BaseContext Builder::EndDict() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Incorrect use of a method 'EndDict' in context. Unmatched '}'");
        }
        nodes_stack_.pop_back();
        if (nodes_stack_.empty()) {
            is_end = true;
        }
        return BaseContext(*this);
    }

    BaseContext Builder::EndArray() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsArray()) {
            throw std::logic_error("Incorrect use of a method 'EndArray' in context. Unmatched ']'");
        }
        nodes_stack_.pop_back();
        if (nodes_stack_.empty()) {
            is_end = true;
        }
        return BaseContext(*this);
    }

    Node Builder::Build() {
        if (nodes_stack_.size() > 0 || root_ == nullptr) {
            throw std::logic_error("Incorrect use of a method 'Build' in context. Unmatched '{'");
        }
        return root_;
    }

    void Builder::SetDict(Node::Value value) {
        if (std::holds_alternative<int>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<int>(value);
        }
        else if (std::holds_alternative<double>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<double>(value);
        }
        else if (std::holds_alternative<bool>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<bool>(value);
        }
        else if (std::holds_alternative<Dict>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<Dict>(value);
        }
        else if (std::holds_alternative<Array>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<Array>(value);
        }
        else if (std::holds_alternative<std::string>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<std::string>(value);
        }
        else if (std::holds_alternative<nullptr_t>(value)) {
            nodes_stack_.back()->AsDict()[*current_key] = std::get<nullptr_t>(value);
        }
    }

    void Builder::SetArray(Node::Value value) {
        if (std::holds_alternative<int>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<int>(value));
        }
        else if (std::holds_alternative<double>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<double>(value));
        }
        else if (std::holds_alternative<bool>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<bool>(value));
        }
        else if (std::holds_alternative<Dict>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<Dict>(value));
        }
        else if (std::holds_alternative<Array>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<Array>(value));
        }
        else if (std::holds_alternative<std::string>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<std::string>(value));
        }
        else if (std::holds_alternative<nullptr_t>(value)) {
            nodes_stack_.back()->AsArray().push_back(std::get<nullptr_t>(value));
        }
    }

    void Builder::SetValue(Node::Value value) {
        if (root_ != nullptr) {
            throw std::logic_error("Incorrect use of a method 'Value' in context");
        }
        if (std::holds_alternative<int>(value)) {
            root_ = std::get<int>(value);
        }
        else if (std::holds_alternative<double>(value)) {
            root_ = std::get<double>(value);
        }
        else if (std::holds_alternative<bool>(value)) {
            root_ = std::get<bool>(value);
        }
        else if (std::holds_alternative<Dict>(value)) {
            root_ = std::get<Dict>(value);
        }
        else if (std::holds_alternative<Array>(value)) {
            root_ = std::get<Array>(value);
        }
        else if (std::holds_alternative<std::string>(value)) {
            root_ = std::get<std::string>(value);
        }
        else if (std::holds_alternative<nullptr_t>(value)) {
            root_ = std::get<nullptr_t>(value);
        }
    }

    BaseContext::BaseContext(Builder& builder) : builder_(builder) {}

    BaseContext BaseContext::Value(Node::Value value) {
        return BaseContext(builder_.Value(value));
    }
    KeyItemContext BaseContext::Key(std::string key) {
        return KeyItemContext(builder_.Key(key));
    }
    DictItemContext BaseContext::StartDict() {
        return DictItemContext(builder_.StartDict());
    }
    ArrayItemContext BaseContext::StartArray() {
        return ArrayItemContext(builder_.StartArray());
    }
    BaseContext BaseContext::EndDict() {
        return BaseContext(builder_.EndDict());
    }
    BaseContext BaseContext::EndArray() {
        return BaseContext(builder_.EndArray());
    }
    Node BaseContext::Build() {
        return builder_.Build();
    }

    ArrayItemContext ArrayItemContext::Value(Node::Value value) {
        return BaseContext::Value(std::move(value));
    }

    DictItemContext KeyItemContext::Value(Node::Value value) {
        return BaseContext::Value(std::move(value));
    }
}