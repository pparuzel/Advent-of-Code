import sys

REG_A, REG_B, REG_C = 0, 1, 2


def combo_operand(state, operand):
    '''Combo operands 0 through 3 represent literal values 0 through 3.
    Combo operand 4 represents the value of register A.
    Combo operand 5 represents the value of register B.
    Combo operand 6 represents the value of register C.
    Combo operand 7 is reserved and will not appear in valid programs.'''
    if 0 <= operand <= 3:
        return operand
    if operand == 7:
        raise NotImplementedError('Reserved combo operand 7 used')
    if 4 <= operand <= 6:
        return state['reg'][operand - 4]
    raise NotImplementedError(f'Unknown combo operand {operand}')


def xdv(state, dest):
    operand = state['prog'][state['pc'] + 1]
    numerator = state['reg'][REG_A]
    denominator = 1 << combo_operand(state, operand)
    state['reg'][dest] = numerator // denominator

# opcode 0
def adv(state):
    return xdv(state, REG_A)

# opcode 1
def bxl(state):
    operand = state['prog'][state['pc'] + 1]
    state['reg'][REG_B] ^= operand

# opcode 2
def bst(state):
    operand = state['prog'][state['pc'] + 1]
    state['reg'][REG_B] = combo_operand(state, operand) % 8

# opcode 3
def jnz(state):
    if state['reg'][REG_A] == 0:
        return
    operand = state['prog'][state['pc'] + 1]
    state['next_pc'] = operand

# opcode 4
def bxc(state):
    _operand = state['prog'][state['pc'] + 1]
    state['reg'][REG_B] ^= state['reg'][REG_C]

# opcode 5
def out(state):
    operand = state['prog'][state['pc'] + 1]
    state['outq'].append(str(combo_operand(state, operand) % 8))

# opcode 6
def bdv(state):
    xdv(state, REG_B)

# opcode 7
def cdv(state):
    xdv(state, REG_C)


INSTRUCTIONS = [
    adv,
    bxl,
    bst,
    jnz,
    bxc,
    out,
    bdv,
    cdv,
]


def solve(registers, program):
    # CPU state
    state = dict(
        prog=program,
        pc=0,  # Instruction pointer/program counter
        next_pc=2,
        reg=registers,
        outq=[],
    )
    while True:
        try:
            opcode = program[state['pc']]
        except IndexError:
            break
        instr = INSTRUCTIONS[opcode]
        instr(state)
        # Increment the instruction pointer
        state['pc'] = state['next_pc']
        state['next_pc'] = state['next_pc'] + 2

    outq = state.pop('outq')
    return ','.join(outq), state


def process_input(file):
    lines = [line.strip() for line in file]
    registers = [0, 0, 0]
    program = []
    for line in lines:
        if not line:
            continue
        name, value = line.split(':')
        if 'Register' in name:
            registers[ord(name[-1]) - ord('A')] = int(value)
        elif 'Program' in name:
            program.extend(int(v) for v in value.split(','))

    return dict(
        registers=registers,
        program=program,
    )


def main():
    ans, _state = solve(**process_input(sys.stdin))
    print('Answer:', ans)


def test():
    input_data = '''
    Register A: 729
    Register B: 0
    Register C: 0

    Program: 0,1,5,4,3,0
    '''.splitlines()
    expected = '4,6,3,5,6,3,5,2,1,0'
    assert solve(**process_input(input_data))[0] == expected


def smoke_tests():
    assert solve([0, 0, 9], [2, 6])[1]['reg'][REG_B] == 1
    assert solve([10, 0, 0], [5, 0, 5, 1, 5, 4])[0] == '0,1,2'
    assert solve([2024, 0, 0], [0, 1, 5, 4, 3, 0])[0] == '4,2,5,6,7,7,7,7,3,1,0'
    assert solve([2024, 0, 0], [0, 1, 5, 4, 3, 0])[1]['reg'][REG_A] == 0
    assert solve([0, 29, 0], [1, 7])[1]['reg'][REG_B] == 26
    assert solve([0, 2024, 43690], [4, 0])[1]['reg'][REG_B] == 44354
    test()

if __name__ == '__main__':
    smoke_tests()
    main()
