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
        void SetDict(Node::Value value);
        void SetArray(Node::Value value);
        void SetValue(Node::Value value);
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