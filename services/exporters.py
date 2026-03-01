import logging
logger = logging.getLogger(__name__)

def export_run_txt(result, filename="data/run_debug.txt"):
    try:
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
                for individual in pop:
                    line = "\t".join(map(str, individual))
                    f.write(line + "\n")
                    
        logger.info(f"Report exported successfully in: {filename}")
    except IOError as e:
        logger.error(f"Something went wrong while writing report {filename}: {e}")
