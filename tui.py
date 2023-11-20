import curses
import concurrent.futures
from run_all import Runner
from enum import Enum, auto
from itertools import cycle
import csv
import logging
import sys

class Modes(Enum):
    Speed = auto(0)
    Size = auto()
    RelSpeed = auto()
    RelSize = auto()


class Benches(Enum):
    AudioMark = auto(0)
    CoreMark = auto()
    EmBench = auto()


SUBS = {
    "EmBench": [
        "aha-mont64",
        "crc32",
        "cubic",
        "edn",
        "huffbench",
        "matmult-int",
        "md5sum",
        "minver",
        "nbody",
        "nettle-aes",
        "nettle-sha256",
        "nsichneu",
        "picojpeg",
        "primecount",
        "qrduino",
        "sglib-combined",
        "slre",
        "st",
        "statemate",
        "tarfind",
        "ud",
        "wikisort",
    ]
}

# Think of this like a constexpr even though it really isn't (Python sucks)
def iter_subs():
    for suite, benches in SUBS.items():
        for bench in benches:
            yield suite, bench


def len_subs():
    return sum(1 for _ in iter_subs())

class Tui():
    WIDTH = 12
    # Suites with multiple executables
    DETAILED = [Benches.EmBench]

    def __init__(self, stdscr):
        # Curses setup
        curses.curs_set(0)
        self.stdscr = stdscr
        self.stdscr.clear()
        # No data yet
        self.col = 0
        self.baseline_col = 0
        self.labels = self.init_labels()
        self.data = self.init_array()
        self.modes_cycle = cycle(Modes)
        self.detail_cycle = cycle(self.DETAILED)
        self.mode = next(self.modes_cycle)
        self.detail = next(self.detail_cycle)
        assert self.mode == Modes.Speed
        self.repo_version = Runner().get_versions()[0]
        self.cc_ids = []
        # Initial UI setup
        self.draw_array()
        self.set_status("", help=True)
        self.stdscr.refresh()

    def cycle_mode(self):
        self.mode = next(self.modes_cycle)
    
    def cycle_detail(self):
        self.detail = next(self.detail_cycle)

    def cycle_baseline(self, right):
        self.baseline_col = self.baseline_col + 1 if right else self.baseline_col - 1
        self.baseline_col = max(0, min(self.baseline_col, self.col - 1))

    def draw_array(self):
        l = self.WIDTH-1
        self.clear_line(0)
        # Print mode in the corner
        self.stdscr.addstr(0, 0, f"{self.mode.name}")
        # Print CC commit hash and options as column labels
        for j in range(self.col):
            self.stdscr.addstr(0, (j + 1) * self.WIDTH, self.cc_ids[j][0][:l]) 
            self.stdscr.addstr(1, (j + 1) * self.WIDTH, self.cc_ids[j][1][:l])

        for i, row in enumerate(self.data[self.mode.value]):
            # Print benchmark names as row labels
            self.stdscr.addstr(i + 2, 0, self.labels[i][:l])
            for j, value in enumerate(row):
                if value is not None:
                    try:
                        # Print benchmark result value
                        logging.debug(type(value))
                        # f-strings really, really wouldn't work here
                        str_val = ("{:" + str(l) + ".6}").format(value) if type(value) == float else str(value).rjust(l)
                        logging.debug('_'+ str_val + '_')
                        # We have to write the full width every time to clear the old value
                        # TODO flush the screen instead?
                        assert len(str_val) == l, "Formatting error"
                        self.stdscr.addstr(i + 2, (j + 1) * self.WIDTH, str_val)
                    except curses.error as e:
                        curses.endwin()
                        print("curses error, terminal probably too narrow")
                        filename = 'output.csv' # TODO
                        self.dump_csv(filename)
                        print(f"Data exported to {filename}")
                        exit(1)


    def clear_line(self, line):
        _, width = self.stdscr.getmaxyx()
        # Overwrite the line with spaces
        self.stdscr.addstr(line, 0, " " * width)

    # Change bottom help/status line
    def set_status(self, status, help):
        height, _ = self.stdscr.getmaxyx()
        end = height - 1
        
        self.clear_line(end - 1)
        self.stdscr.addstr(end - 1, 0, status)
        if help:
            self.stdscr.addstr(end, 0, "Press q to terminate, r to run benchmarks, m to change mode")
        self.stdscr.refresh()


    def init_labels(self):
        labels = []
        for b in Benches.__members__:
            labels.append(b)
        for b in iter_subs():
            labels.append(f"{b[0][:2]}_{b[1]}")
        return labels


    def init_array(self):
        return [[[] for _ in range(len(Benches) + len_subs())] for _ in Modes]


    def init_details(self):
        # data = [[[None]*cols for _ in Benches] for _ in Modes]
        # for mode_array in data:
        #     for b in Benches.__members__:
        #         for c in range(cols):
        #             mode_array[Benches[b].value][c] = '...'
        assert False
        return []
        # return data


    def add_col(self):
        for mode_array in self.data:
            for i in range(len(Benches) + len_subs()):
                mode_array[i].append('...')
        self.col += 1


    def set_done(self, num_done):
        self.set_status(f"Running benchmarks... {num_done}/{len(Benches)}", help=False)


    def update_relative(self):
        def percent(x):
            return f"{x:.1%}"

        for c in range(self.col):
            for i in range(len(Benches) + len_subs()):
                speed_data = self.data[Modes.Speed.value][i]
                size_data = self.data[Modes.Size.value][i]
                rel_speed_data = self.data[Modes.RelSpeed.value][i]
                rel_size_data = self.data[Modes.RelSize.value][i]
                rel_speed_data[c] = percent(float(speed_data[c]) / float(speed_data[self.baseline_col]))
                rel_size_data[c] = percent(float(size_data[c]) / float(size_data[self.baseline_col]))

    def run_all(self):
        # This also reloads the environment file
        runner = Runner()
        # Include paths are boring, filter them out
        perf_opts = ' '.join(list(filter(lambda x: '-I' not in x, runner.CFLAGS.split(' '))))
        self.cc_ids.append((runner.get_versions()[1], perf_opts))
        # New column for this run
        self.add_col()
        done = 0
        # Initialize progress counter
        self.set_done(done)

        # Run the benchmark simulations in parallel
        with concurrent.futures.ThreadPoolExecutor() as executor:
            futures = [executor.submit(runner.run_bench, b) for b in Benches.__members__]
            # Iterate over results as we get them
            for future in concurrent.futures.as_completed(futures):
                res = future.result()
                if res:
                    b, (speeds, sizes) = future.result()
                    # Get a "mutable reference" to a row (Python sucks)
                    speed_data = self.data[Modes.Speed.value][Benches[b].value]
                    size_data = self.data[Modes.Size.value][Benches[b].value]
                    # Fill in the corresponding fields in the new column
                    if Benches[b] in self.DETAILED:
                        # TODO use self.detail cycle
                        speed_data[self.col - 1] = speeds[0][1]
                        size_data[self.col - 1] = sizes[0][1]
                        for ((sub_name, speed), (_, size)) in zip(speeds, sizes):
                            if sub_name == 'geometric mean':
                                continue
                            size_data[self.col - 1] = sizes[0][1]
                            name = f"{Benches[b]}_{sub_name}"
                            # logging.debug(f"{name}:{speed}:{size}")
                            idx = len(Benches) + list(iter_subs()).index((Benches[b].name, sub_name))
                            self.data[Modes.Speed.value][idx][self.col - 1] = speed
                            self.data[Modes.Size.value][idx][self.col - 1] = size
                    else:
                        speed_data[self.col - 1] = speeds
                        size_data[self.col - 1] = sizes
                    self.draw_array()
                    done += 1
                    self.set_done(done)
                    self.stdscr.refresh()

        self.update_relative()
        self.set_status("Done!", help=True)


    def dump_csv(self, filename):
        with open(filename, "w", newline="") as csvfile:
            csv_writer = csv.writer(csvfile)
            csv_writer.writerow([f"RISC-V benchmark suite {self.repo_version}"] + self.cc_ids)
            for mode in [Modes.Speed, Modes.Size]:
                csv_writer.writerow([mode])
                mode_array = self.data[mode.value]
                for row in mode_array:
                    csv_writer.writerow(row)


def main(stdscr):
    tui = Tui(stdscr)

    while True:
        stdscr.refresh()
        key = stdscr.getch()
        if key == ord('q') or key == ord('Q'):
            filename = 'output.csv' # TODO
            tui.dump_csv(filename)
            curses.endwin()
            sys.stdout.flush()
            print(f"Data exported to {filename}")
            break

        if key == ord('r') or key == ord('R'):
            tui.run_all()

        if key == ord('m') or key == ord('M'):
            tui.cycle_mode()
            tui.draw_array()
        
        if key == ord('d') or key == ord('D'):
            # TODO
            tui.cycle_detail()
            tui.draw_array()

        if key == curses.KEY_RIGHT:
            tui.cycle_baseline(right=True)
            tui.update_relative()
            tui.draw_array()

        if key == curses.KEY_LEFT:
            tui.cycle_baseline(right=False)
            tui.update_relative()
            tui.draw_array()

if __name__ == "__main__":
    logging.basicConfig(filename='debug.log', level=logging.DEBUG)
    curses.wrapper(main)
