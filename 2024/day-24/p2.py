import sys


AND = 'AND'
OR = 'OR'
XOR = 'XOR'


def half_adder(a, b, out, carry):
    return {out: (a, XOR, b), carry: (a, AND, b)}


def full_adder(a, b, c, axorb, axorb_andc, aandb, out, carry):
    return {axorb: (a, XOR, b),
            axorb_andc: (axorb, AND, c),
            aandb: (a, AND, b),
            out: (axorb, XOR, c),
            carry: (axorb_andc, OR, aandb)}


def main():
    for line in sys.stdin:
        if not line.split(':')[0].lstrip():
            # Ignore register values
            break

    w2out = {}
    out2w = {}
    for line in sys.stdin:
        a, op, b, arrow, out = line.split(' ')
        assert arrow == '->'
        out = out.rstrip()
        w2out[a, op, b] = out
        w2out[b, op, a] = out
        out2w[out] = (a, op, b)

    def get_full_adder(a, b, cin, fa):
        '''
        Full Adder Schema
        C - - - - xor - - - - - - Output
                  |
        A - xor - + - and - or  - Carry
            |         |     |
        B - +         |     |
                      |     |
        C - - - - - - +     |
                            |
        B - - - - - - and - +
                      |
        A - - - - - - +

        z45 == carry44 z44 == output44
        '''
        print({
            'a': a,
            'b': b,
            'cin': cin,
            })
        axorb = w2out[a, XOR, b]
        aandb = w2out[a, AND, b]
        axorb_andc = w2out[axorb, AND, cin]
        axorb_xorc = w2out[axorb, XOR, cin]
        or1 = w2out[axorb_andc, OR, aandb]
        fa.update({
            'xor': f'{a} xor {b} = {axorb}',
            'out': f'{axorb} xor {cin} = {axorb_xorc}',
            'and1': f'{a} and {b} = {aandb}',
            'and2': f'{axorb} and {cin} = {axorb_andc}',
            'cout': f'{axorb_andc} or {aandb} = {or1}',
        })
        # S, Cout
        return axorb_xorc, or1

    # Validate half adder params
    assert w2out[('x00', 'XOR', 'y00')] == 'z00'
    c01 = w2out[('x00', 'AND', 'y00')]
    c_out = c01

    fa = [{}] * 44  # full adders

    for i in range(1, 44):
        zz = f'z{i:02}'
        print(f'i={i}', f'{zz} <= {out2w[zz]}')
        zi, c_out = get_full_adder(f'x{i:02}', f'y{i:02}', c_out, fa[i])
        # assert zi == f'z{i:02}', f'z{i:02} expected but got: {zi}'
        print(zi, c_out)
        print(fa[i])


if __name__ == '__main__':
    main()
