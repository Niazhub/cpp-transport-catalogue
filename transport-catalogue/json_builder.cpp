#include "json_builder.h"

namespace json {
    BaseContext Builder::Value(Node::Value value) {
        if (is_end) {
            throw std::logic_error("Incorrect use of a method 'Value' in this context");
        }
        if (!nodes_stack_.empty()) {
            if (current_key) {
                nodes_stack_.back()->AsDict()[*current_key] = CheckerValue(value);
                current_key.reset();
            }
            else if (nodes_stack_.back()->IsArray()) {
                nodes_stack_.back()->AsArray().push_back(CheckerValue(value));
            }
            else if (nodes_stack_.back()->IsDict()) {
                throw std::logic_error("Incorrect use of a method 'Value' in this context");
            }
        }
        else {
            root_ = CheckerValue(value);
        }
        return BaseContext(*this);
    }

    KeyItemContext Builder::Key(std::string key) {
        if (is_end || current_key.has_value() || nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Incorrect use of a method 'Key' in this context");
        }
        current_key = std::move(key);
        return KeyItemContext(*this);
    }

    DictItemContext Builder::StartDict() {
        StartContainer(Dict{});
        return DictItemContext(*this);
    }

    ArrayItemContext Builder::StartArray() {
        StartContainer(Array{});
        return ArrayItemContext(*this);
    }

    BaseContext Builder::EndDict() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsDict()) {
            throw std::logic_error("Incorrect use of a method 'EndDict' in this context. Unmatched '}'");
        }
        nodes_stack_.pop_back();
        if (nodes_stack_.empty()) {
            is_end = true;
        }
        return BaseContext(*this);
    }

    BaseContext Builder::EndArray() {
        if (nodes_stack_.empty() || !nodes_stack_.back()->IsArray()) {
            throw std::logic_error("Incorrect use of a method 'EndArray' in this context. Unmatched ']'");
        }
        nodes_stack_.pop_back();
        if (nodes_stack_.empty()) {
            is_end = true;
        }
        return BaseContext(*this);
    }

    Node Builder::Build() {
        if (nodes_stack_.size() > 0 || root_ == nullptr) {
            throw std::logic_error("Incorrect use of a method 'Build' in this context. Unmatched '{'");
        }
        return root_;
    }

    Node Builder::CheckerValue(Node::Value value) {
        if (std::holds_alternative<int>(value)) {
            return std::get<int>(value);
        }
        else if (std::holds_alternative<double>(value)) {
            return std::get<double>(value);
        }
        else if (std::holds_alternative<bool>(value)) {
            return std::get<bool>(value);
        }
        else if (std::holds_alternative<Dict>(value)) {
            return std::get<Dict>(value);
        }
        else if (std::holds_alternative<Array>(value)) {
            return std::get<Array>(value);
        }
        else if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        }
        else if (std::holds_alternative<nullptr_t>(value)) {
            return std::get<nullptr_t>(value);
        }
        throw std::logic_error("Unsupported value type");
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