#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"

namespace vm
{

    vm_state create_vm(bool debug)
    {
        vm_state state;

        // enable vm debugging
        state.debug = debug;

        register_instruction(state, "LOAD_CONST", [](vm_state &vmstate, const item_t arg)
                             {
        vmstate.stack.push(arg);
        return true; });

        register_instruction(state, "PRINT", [](vm_state &vmstate, const item_t /*arg*/)
                             {
        std::cout << vmstate.stack.top() << std::endl;
        return true; });

        register_instruction(state, "EXIT", [](vm_state &vmstate, const item_t /*arg*/)
                             {
        if (vmstate.stack.empty())
            throw vm_stackfail{"EXIT: Empty stack"};
        vmstate.result = vmstate.stack.top();
        return false; });

        register_instruction(state, "POP", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.empty()) {
        throw vm_stackfail{"POP: Empty stack"};
    }
    vmstate.stack.pop();
    return true; });

        register_instruction(state, "ADD", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.size() < 2) {
        throw vm_stackfail{"ADD: Requires at least two items on the stack"};
    }
        item_t a = vmstate.stack.top();
        vmstate.stack.pop();
        item_t b = vmstate.stack.top();
        vmstate.stack.pop();

        vmstate.stack.push(a + b);
        return true; });

        register_instruction(state, "DIV", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.size() < 2) {
        throw vm_stackfail{"DIV: Requires at least two items on the stack"};
    }
    item_t divisor = vmstate.stack.top();
    vmstate.stack.pop();
    item_t dividend = vmstate.stack.top();
    vmstate.stack.pop();
    if (divisor == 0) {
        throw div_by_zero{"DIV: Division by zero"};
    }
    vmstate.stack.push(dividend / divisor);
    return true; });

        register_instruction(state, "EQ", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.size() < 2) {
        throw vm_stackfail{"EQ: Requires at least two items on the stack"};
    }
    item_t a = vmstate.stack.top();
    vmstate.stack.pop();
    item_t b = vmstate.stack.top();
    vmstate.stack.pop();
    vmstate.stack.push((a == b) ? 1 : 0);
    return true; });

        register_instruction(state, "NEQ", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.size() < 2) {
        throw vm_stackfail{"NEQ: Requires at least two items on the stack"};
    }
    item_t a = vmstate.stack.top();
    vmstate.stack.pop();
    item_t b = vmstate.stack.top();
    vmstate.stack.pop();
    vmstate.stack.push((a != b) ? 1 : 0);
    return true; });

        register_instruction(state, "DUP", [](vm_state &vmstate, const item_t /*arg*/)
                             {
        if (vmstate.stack.empty()) {
            throw vm_stackfail{"DUP: Empty stack"};
        }
        item_t tos = vmstate.stack.top();
        vmstate.stack.push(tos);
        return true; });

        register_instruction(state, "JMP", [](vm_state &vmstate, const item_t arg)
                             {
    vmstate.pc = static_cast<size_t>(arg);
    return true; });

        register_instruction(state, "JMPZ", [](vm_state &vmstate, const item_t arg)
                             {
    if (vmstate.stack.empty()) {
        throw vm_stackfail{"JMPZ: Empty stack"};
    }
    item_t tos = vmstate.stack.top();
    vmstate.stack.pop();
    if (tos == 0) {
        vmstate.pc = static_cast<size_t>(arg);
    }
    return true; });

        register_instruction(state, "WRITE", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.empty()) {
        throw vm_stackfail{"WRITE: Empty stack"};
    }
    item_t tos = vmstate.stack.top();
    vmstate.output += std::to_string(tos);
    return true; });

        register_instruction(state, "WRITE_CHAR", [](vm_state &vmstate, const item_t /*arg*/)
                             {
    if (vmstate.stack.empty()) {
        throw vm_stackfail{"WRITE CHAR: Empty stack"};
    }
    item_t tos = vmstate.stack.top();
    vmstate.output += static_cast<char>(tos);
    return true; });

        return state;
    }

    void register_instruction(vm_state &state, std::string_view name,
                              const op_action_t &action)
    {
        size_t op_id = state.next_op_id;

        state.instruction_ids[static_cast<std::string>(name)] = op_id;
        state.instruction_names[op_id] = static_cast<std::string>(name);
        state.instruction_actions[op_id] = action;

        // increment the next_op_id for the next instruction
        state.next_op_id += 1;
    }

    code_t assemble(const vm_state &state, std::string_view input_program)
    {
        code_t code;

        // convert each line separately
        for (auto &line : util::split(input_program, '\n'))
        {

            auto line_words = util::split(line, ' ');

            // only support instruction and one argument
            if (line_words.size() >= 3)
            {
                throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
            }

            // look up instruction id
            auto &op_name = line_words[0];
            auto find_op_id = state.instruction_ids.find(op_name);
            if (find_op_id == std::end(state.instruction_ids))
            {
                throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
            }
            op_id_t op_id = find_op_id->second;

            // parse the argument
            item_t argument{0};
            if (line_words.size() == 2)
            {
                argument = std::stoll(line_words[1]);
            }

            // and save the instruction to the code store
            code.emplace_back(op_id, argument);
        }

        return code;
    }

    std::tuple<item_t, std::string> run(vm_state &vm, const code_t &code)
    {
        // to help you debugging the code!
        if (vm.debug)
        {
            std::cout << "=== running vm ======================" << std::endl;
            std::cout << "disassembly of run code:" << std::endl;
            for (const auto &[op_id, arg] : code)
            {
                if (not vm.instruction_names.contains(op_id))
                {
                    std::cout << "could not disassemble - op_id unknown..." << std::endl;
                    std::cout << "turning off debug mode." << std::endl;
                    vm.debug = false;
                    break;
                }
                std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
            }
            std::cout << "=== end of disassembly" << std::endl
                      << std::endl;
        }

        // execution loop for the machine
        while (true)
        {

            auto &[op_id, arg] = code[vm.pc];

            if (vm.debug)
            {
                std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
            }

            // increase the program counter here so its value can be overwritten
            // by the instruction when it executes!
            vm.pc += 1;

            // Check if the program counter is within the bounds of the code vector
            if (vm.pc > code.size())
            {
                throw vm_segfault{"Non-existent program address"};
            }

            // Execute the instruction and stop if the action returns false.
            if (!vm.instruction_actions[op_id](vm, arg))
            {
                break;
            }
        }

        return {vm.result, vm.output};
    }

} // namespace vm
