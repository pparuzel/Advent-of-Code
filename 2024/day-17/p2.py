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
    state['outq'].append(combo_operand(state, operand) % 8)

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

    return ','.join(map(str, state['outq'])), state


def execute(program, registers):
    return solve(registers, program)[1]['outq']


def main():
    for line in sys.stdin:
        if line.startswith('Program:'):
            program = list(int(v) for v in line.removeprefix('Program:')
                                               .strip().split(','))
            break
    else:
        print('error: Could not find a program input line', file=sys.stderr)
        sys.exit(255)

    ans = find_selfgen(program, 0, len(program) - 1)
    print(f'Answer: {ans}')


def find_selfgen(program, input_data, pos):
    if pos < 0:
        return input_data
    # Build the input value incrementally in reverse
    input_data <<= 3
    for i in range(8):  # 3-bit range
        output = execute(program, [input_data | i, 0, 0])
        if output == program[pos:]:
            found = find_selfgen(program, input_data | i, pos - 1)
            if found is not None:
                return found
    return None


if __name__ == '__main__':
    main()
