#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
    return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(const Token &token) {
    // if (std::holds_alternative<state::Valid>(state_)) {
    //     state_ = transition(std::get<state::Valid>(state_), token);
    // } else if (std::holds_alternative<state::Invalid>(state_)) {
    //     state_ = transition(std::get<state::Invalid>(state_), token);
    // } else if (std::holds_alternative<state::Start>(state_)) {
    //     state_ = transition(std::get<state::Start>(state_), token);
    // } else if (std::holds_alternative<state::SelectStmt>(state_)) {
    //     state_ = transition(std::get<state::SelectStmt>(state_), token);
    // } else if (std::holds_alternative<state::AllColumns>(state_)) {
    //     state_ = transition(std::get<state::AllColumns>(state_), token);
    // } else if (std::holds_alternative<state::NamedColumn>(state_)) {
    //     state_ = transition(std::get<state::NamedColumn>(state_), token);
    // } else if (std::holds_alternative<state::MoreColumns>(state_)) {
    //     state_ = transition(std::get<state::MoreColumns>(state_), token);
    // } else if (std::holds_alternative<state::FromClause>(state_)) {
    //     state_ = transition(std::get<state::FromClause>(state_), token);
    // } else if (std::holds_alternative<state::TableName>(state_)) {
    //     state_ = transition(std::get<state::TableName>(state_), token);
    // }
    state_ = std::visit([&](auto cur) -> State { return transition(cur, token); }, this->state_);
}


struct TransitionFromStartVisitor {
    // Only the `Select` token advances the FSM from the `Start` state
    // TODO: Correct the behaviour
    State operator()(token::Select) const { return state::SelectStmt{}; }

    // All the other tokens, put it in the invalid state
    State operator()(token::From) const { return state::Invalid{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
    State operator()(const auto&) const { return state::Invalid{}; } // Default case for other tokens

};

struct TransitionFromValidVisitor {
    State operator()(token::Semicolon) const { return state::Valid{}; }
    State operator()(const auto&) const { return state::Invalid{}; } 
};

struct TransitionFromInvalidVisitor {
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromSelectStmtVisitor {
    State operator()(token::Asterisks) const { return state::AllColumns{}; }
    State operator()(token::Identifier) const { return state::NamedColumn{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromAllColumnsVisitor {
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
    State operator()(token::Comma) const { return state::MoreColumns{}; }
    State operator()(token::From) const { return state::FromClause{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromMoreColumnsVisitor {
    State operator()(token::Identifier) const { return state::NamedColumn{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromFromClauseVisitor {
    State operator()(token::Identifier) const { return state::TableName{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

struct TransitionFromTableNameVisitor {
    State operator()(token::Semicolon) const { return state::Valid{}; }
    State operator()(const auto&) const { return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, const Token &token) {
    return std::visit(TransitionFromValidVisitor{}, token.value());
}

State transition(state::Invalid, const Token &token) {
    return std::visit(TransitionFromInvalidVisitor{}, token.value());
}

State transition(state::SelectStmt, const Token &token) {
    return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}

State transition(state::AllColumns, const Token &token) {
    return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}

State transition(state::NamedColumn, const Token &token) {
    return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

State transition(state::MoreColumns, const Token &token) {
    return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

State transition(state::FromClause, const Token &token) {
    return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

State transition(state::TableName, const Token &token) {
    return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

bool is_valid_sql_query(const std::vector<Token> &tokens) {
    SqlValidator validator;

    for (const auto &token : tokens) {
        validator.handle(token);
    }

    // Check conditions for a valid SQL query
    if (validator.is_valid()) {
        return true;
    }

    return false;
}
} // namespace sql