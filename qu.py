import sys


SCALE = 50
SVG_HEADER = '<svg version="1.1" baseProfile="full" width="W" height="H" xmlns="http://www.w3.org/2000/svg">\n'
SVG_FOOTER = '</svg>'

rangle = "\u3009"
pipe = "\u01c0"
psi = "\u03a8"

def line(x1, y1, x2, y2, stroke="black", thickness=None):
    s = f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="{stroke}" '
    if thickness is not None:
        s += f'stroke-width="{thickness}" '
    s += '/>\n'
    return s


def rect(x, y, width=40, height=40, stroke="black", fill="#eeeeee"):
    return f'<rect x="{x}" y="{y}" width="{width}" height="{height}" stroke="{stroke}" fill="{fill}" />\n'


def text(x, y, value, font="monospace"):
    return f'<text font-family="{font}" x="{x}" y="{y}" text-anchor="middle">{value}</text>\n'


def circle(x, y, radius, fill="black", stroke="black"):
    return f'<circle cx="{x}" cy="{y}" r="{radius}" fill="{fill}" stroke="{stroke}" />\n'


class Component:
    def __init__(self):
        self.x = 0
        self.y = 0

    def atx(self, spaces):
        self.x = SCALE * spaces
        return self
    
    def aty(self, spaces):
        self.y = SCALE * spaces
        return self

    def at(self, x_spaces, y_spaces):
        self.atx(x_spaces)
        self.aty(y_spaces)
        return self

    def draw(self):
        raise NotImplementedError


class SingleQubitGate(Component):
    def __init__(self, label):
        super().__init__()
        self.label = label

    def draw(self):
        s = rect(self.x+5, self.y+5)
        s += text(self.x+25, self.y+29, self.label)
        return s


class Wire(Component):
    def __init__(self):
        super().__init__()

    def draw(self):
        s = line(self.x, self.y+25, self.x+50, self.y+25)
        return s


class ClassicalWire(Component):
    def __init__(self):
        super().__init__()
    
    def draw(self):
        s = line(self.x, self.y+23, self.x+50, self.y+23)
        s += line(self.x, self.y+25, self.x+50, self.y+25, thickness=3, stroke="#eeeeee")
        s += line(self.x, self.y+27, self.x+50, self.y+27)
        return s


class Label(Component):
    def __init__(self, label):
        super().__init__()
        self.label = label

    def draw(self):
        s = rect(self.x, self.y, width=SCALE, height=SCALE, stroke="#eeeeee")
        s += text(self.x+25, self.y+29, self.label)
        return s


class Space(Component):
    def __init__(self):
        super().__init__()

    def draw(self):
        s = rect(self.x, self.y, width=50, height=50, stroke="#eeeeee")
        return s

class Measurement(Component):
    def __init__(self):
        super().__init__()

    def draw(self):
        s = line(self.x+45, self.y+23, self.x+50, self.y+23)
        s += line(self.x+45, self.y+25, self.x+50, self.y+25, thickness=3, stroke="#eeeeee")
        s += line(self.x+45, self.y+27, self.x+50, self.y+27)
        s += rect(self.x+5, self.y+5)
        s += f'<path d="M {self.x+10} {self.y+30} A 20 20 180 0 1 {self.x+40} {self.y+30}" stroke="black" fill="#eeeeee" />'
        s += line(self.x+25, self.y+30, self.x+35, self.y+15)
        return s


class Control(Component):
    def __init__(self, label, size):
        super().__init__()
        self.label = label
        self.size = size

    def draw(self):
        s = line(self.x+25, self.y+25, self.x+25, self.y+25+self.size*SCALE)
        s += rect(self.x+5, self.y+SCALE*self.size+5)
        s += circle(self.x+25, self.y+25, 3)
        s += text(self.x+25, self.y+SCALE*self.size+29, self.label)
        return s


class MultiQubitGate(Component):
    def __init__(self, label: str, size: int):
        super().__init__()
        self.label = label
        self.size = size

    def draw(self):
        s = rect(self.x+5, self.y+5, height=self.size*SCALE-10)
        s += text(self.x+25, self.y+29, self.label)
        return s


class CNOT(Component):
    def __init__(self, size: int):
        super().__init__()
        self.size = size

    def draw(self):
        s = line(self.x+25, self.y+25, self.x+25, self.y+25+self.size*SCALE)
        s += circle(self.x+25, self.y+25, 3)
        s += circle(self.x+25, self.y+25+SCALE*self.size, 5, fill="white")
        s += line(self.x+25-5, self.y+25+SCALE*self.size, self.x+25+5, self.y+25+SCALE*self.size)
        s += line(self.x+25, self.y+25+SCALE*self.size-5, self.x+25, self.y+25+SCALE*self.size+5)
        return s


els = {
    "X": SingleQubitGate("X"),
    "Y": SingleQubitGate("Y"),
    "Z": SingleQubitGate("Z"),
    "X(2)": MultiQubitGate("X", 2),
    "CNOT": CNOT(1),
    "CNOT(1)": CNOT(1),
    "CNOT(-1)": CNOT(-1),
    "CNOT(2)": CNOT(2),
    "CNOT(-2)": CNOT(-2),
    "H": SingleQubitGate("H"),
    "wire": Wire(),
    "cwire": ClassicalWire(),
    "label(0)": Label("|0"+rangle),
    "psi": Label("|"+psi+rangle),
    "U": SingleQubitGate("U"),
    "M": Measurement(),
    "space": Space()
}


if __name__ == "__main__":
    if len(sys.argv) != 2:
        input_file = "input.txt"
    else:
        input_file = sys.argv[1]

    ## Get the input string and parse some particulars.
    with open(input_file, "r") as f:
        lines = f.readlines()
        lines = [x.strip() for x in lines]
        height = len(lines)
        width = max([len(l.split()) for l in lines])

    with open("output.svg", "w") as f:
        ## Write SVG header.
        f.write(SVG_HEADER.replace("W",str(width*SCALE)).replace("H", str(height*SCALE)))

        ## Draw the grid of wires.
        wire = Wire()        
        for i in range(width):
            for j in range(height):
                f.write(wire.at(i, j).draw())

        for num, ln in enumerate(lines):
            for index, value in enumerate(ln.split()):
                if value != "wire":
                    if value in els:
                        f.write(els[value].at(index, num).draw())
                    elif "multi" in value:
                        dash_indices = [d for d, _ in enumerate(list(value)) if _ == "-"]
                        label = value[dash_indices[0]+1:dash_indices[1]]
                        coverage = int(value[dash_indices[1]+1:])
                        gate = MultiQubitGate(label, coverage)
                        f.write(gate.at(index, num).draw())
                    elif "label" in value:
                        bracket_index = value.index("(")
                        label = Label("|"+value[bracket_index+1:-1]+rangle)
                        f.write(label.at(index, num).draw())
                    elif "control" in value:
                        dash_indices = [d for d, _ in enumerate(list(value)) if _ == "-"]
                        label = value[dash_indices[0]+1:dash_indices[1]]
                        coverage = int(value[dash_indices[1]+1:])
                        gate = Control(label, coverage)
                        f.write(gate.at(index, num).draw())
                    elif "CNOT" in value:
                        bracket_index = value.index("(")
                        size = int(value[bracket_index+1:-1])
                        gate = CNOT(size)
                        f.write(gate.at(index, num).draw())
                    else:
                        gate = SingleQubitGate(value)
                        f.write(gate.at(index, num).draw())
                        



        ## Write SVG footer.
        f.write(SVG_FOOTER)
