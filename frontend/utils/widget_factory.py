from PySide6.QtWidgets import QSpinBox, QDoubleSpinBox
        
def create_spin_box(
        min_val=0, 
        max_val=100, 
        step=10, 
        value=None,
        enable=True
    ):
        sb = QSpinBox()
        sb.setRange(min_val, max_val)
        sb.setSingleStep(step)
        sb.setEnabled(enable)
        
        if value is not None:
            sb.setValue(value)
            
        return sb
        
def create_double_spin_box(
        min_val=0, 
        max_val=100, 
        step=10, 
        value=None,
        decimals=2,
        enable=True
    ):
        dsb = QDoubleSpinBox()
        dsb.setRange(min_val, max_val)
        dsb.setSingleStep(step)
        dsb.setDecimals(decimals)
        dsb.setEnabled(enable)
        
        if value is not None:
            dsb.setValue(value)
            
        return dsb
