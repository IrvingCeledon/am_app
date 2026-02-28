from dataclasses import dataclass, field

@dataclass
class RunResult:
    algorithm: str
    params: dict = field(default_factory=dict)
    metrics: dict = field(default_factory=dict)
    snapshots: dict = field(default_factory=dict)
