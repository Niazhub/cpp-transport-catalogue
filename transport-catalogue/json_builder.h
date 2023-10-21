#pragma once

#include "json.h"

#include <optional>

namespace json {

    class BaseContext;
    class DictItemContext;
    class ArrayItemContext;
    class KeyItemContext;

    class Builder {
    public:
        BaseContext Value(Node::Value value);
        KeyItemContext Key(std::string key);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        BaseContext EndDict();
        BaseContext EndArray();
        Node Build();

    private:
        Node root_;
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> current_key;
        bool is_end = false;
        Node CheckerValue(Node::Value value);

        template <typename T>
        void StartContainer(T container) {
            if (is_end) {
                if (typeid(T) == typeid(Dict))
                    throw std::logic_error("Incorrect use of a method 'StartDict' in context");
                else if (typeid(T) == typeid(Array))
                    throw std::logic_error("Incorrect use of a method 'StartArray' in context");
            }
            if (nodes_stack_.empty()) {
                root_ = container;
                nodes_stack_.push_back(&root_);
            }
            else {
                auto new_root = Dict{};
                if (current_key) {
                    if (typeid(T) == typeid(Dict)) {
                        nodes_stack_.back()->AsDict()[*current_key] = std::move(new_root);
                        nodes_stack_.emplace_back(&nodes_stack_.back()->AsDict()[*current_key]);
                    }
                    else if (typeid(T) == typeid(Array)) {
                        root_.AsDict()[*current_key] = Array{};
                        nodes_stack_.emplace_back(&root_.AsDict()[*current_key]);
                    }
                    current_key.reset();
                }
                else {
                    if (typeid(T) == typeid(Dict)) {
                        nodes_stack_.back()->AsArray().emplace_back(std::move(new_root));
                    }
                    else if (typeid(T) == typeid(Array)) {
                        nodes_stack_.back()->AsArray().push_back(std::move(root_));
                    }
                    nodes_stack_.emplace_back(&nodes_stack_.back()->AsArray().back());
                }
            }
        }
    };

    class BaseContext {
    public:
        BaseContext(Builder& builder);

        BaseContext Value(Node::Value value);
        KeyItemContext Key(std::string key);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        BaseContext EndDict();
        BaseContext EndArray();
        Node Build();
    private:
        Builder& builder_;
    };

    class DictItemContext : public BaseContext {
    public:
        DictItemContext(BaseContext base) : BaseContext(base) {}

        BaseContext Value(Node::Value value) = delete;
        DictItemContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
        BaseContext EndArray() = delete;
        Node Build() = delete;
    };

    class ArrayItemContext : public BaseContext {
    public:
        ArrayItemContext(BaseContext base) : BaseContext(base) {}

        ArrayItemContext Value(Node::Value value);
        KeyItemContext Key(std::string key) = delete;
        BaseContext EndDict() = delete;
        Node Build() = delete;
    };

    class KeyItemContext : public BaseContext {
    public:
        KeyItemContext(BaseContext base) : BaseContext(base) {}

        DictItemContext Value(Node::Value value);
        KeyItemContext Key(std::string key) = delete;
        BaseContext EndDict() = delete;
        BaseContext EndArray() = delete;
        Node Build() = delete;
    };
}