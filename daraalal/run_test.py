import sys
import subprocess
from pathlib import Path

TIMEOUT_SEC = 2.0

def norm(s: str) -> str:
    # CRLF/LF ялгаа, мөрийн төгсгөлийн space-ээс болж унахгүй болгоно
    s = s.replace("\r\n", "\n").replace("\r", "\n")
    lines = [line.rstrip() for line in s.split("\n")]
    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines) + ("\n" if lines else "")

def compile_cpp(src: Path, out_bin: Path) -> None:
    cmd = ["g++", "-std=c++17", "-O2", "-pipe", str(src), "-o", str(out_bin)]
    print("Compiling:", " ".join(cmd))
    subprocess.check_call(cmd)

def run(cmd, inp: str) -> str:
    p = subprocess.run(
        cmd,
        input=inp.encode(),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=TIMEOUT_SEC,
    )
    if p.returncode != 0:
        raise RuntimeError(
            f"Exit code: {p.returncode}\nSTDERR:\n{p.stderr.decode(errors='ignore')}"
        )
    return p.stdout.decode(errors="ignore")

def main():
    root = Path(__file__).resolve().parent
    test = root / "test"

    cpp = root / "solution.cpp"
    py  = root / "solution.py"

    if not test.exists():
        print("Missing test folder:", test)
        return 1

    runner = None
    bin_path = root / "a.out"

    if cpp.exists():
        compile_cpp(cpp, bin_path)
        runner = [str(bin_path)]
    elif py.exists():
        runner = [sys.executable, str(py)]
    else:
        print("Missing solution.cpp or solution.py in", root)
        return 1

    in_files = sorted(test.glob("*.in"))
    if not in_files:
        print("No .in files in", test)
        return 1

    ran = 0
    for in_path in in_files:
        name = in_path.stem
        out_path = test / f"{name}.out"
        if not out_path.exists():
            print(f"[SKIP] {name}: missing {out_path.name}")
            continue

        inp = in_path.read_text(encoding="utf-8")
        expected = out_path.read_text(encoding="utf-8")

        try:
            got = run(runner, inp)
        except subprocess.TimeoutExpired:
            print(f"[FAIL] {name}: TIMEOUT ({TIMEOUT_SEC}s)")
            return 1
        except Exception as e:
            print(f"[FAIL] {name}: {e}")
            return 1

        if norm(got) != norm(expected):
            print(f"[FAIL] {name}")
            print("---- expected ----")
            print(expected)
            print("---- got ----")
            print(got)
            return 1

        print(f"[PASS] {name}")
        ran += 1

    if ran == 0:
        print("No test executed (maybe missing .out files).")
        return 1

    print(f"\nAll test passed: {ran}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
