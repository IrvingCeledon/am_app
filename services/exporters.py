def export_run_txt(result, filename="data/run_debug.txt"):
    with open(filename, "w") as f:

        f.write(f"===== {result.algorithm} REPORT =====\n\n")

        # params
        f.write("Parameters:\n")
        for k, v in result.params.items():
            f.write(f"{k}: {v}\n")

        # metrics
        for name, values in result.metrics.items():
            f.write(f"\n{name}:\n")
            for i, v in enumerate(values, 1):
                f.write(f"{i}\t{v}\n")

        # snapshots
        for label, pop in result.snapshots.items():
            f.write(f"\n{label} population:\n")
            for x, y in pop:
                f.write(f"{x}\t{y}\n")
