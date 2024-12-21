import sys
from functools import cache

AND = 0
OR = 1
XOR = 2

def main():
    wires = {}
    for line in sys.stdin:
        try:
            name, value = line.rstrip().split(':')
        except ValueError:
            break
        wires[name] = dict(op=OR, input1=value.lstrip(), input2='0')
    # print(wires)
    op_to_enum = dict(OR=OR, XOR=XOR, AND=AND)
    for line in sys.stdin:
        wire1, op, wire2, arrow, output = line.rstrip().split(' ')
        assert arrow == '->'
        wires[output] = dict(op=op_to_enum[op], input1=wire1, input2=wire2)

    @cache
    def get_value(wire):
        if wire == '0' or wire == '1':
            return int(wire)
        node = wires[wire]
        in1 = get_value(node['input1'])
        in2 = get_value(node['input2'])
        return {
            AND: in1 & in2,
            OR: in1 | in2,
            XOR: in1 ^ in2,
        }[node['op']]


    prog_out = {w: get_value(w) for w in wires if w.startswith('z')}
    output_wires = sorted(prog_out.keys(), reverse=True)
    output = int(''.join(str(prog_out[wire]) for wire in output_wires), 2)
    print(f'Answer: {output}')


if __name__ == '__main__':
    main()
