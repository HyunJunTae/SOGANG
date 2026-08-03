set_property SEVERITY {Warning} [get_drc_checks NSTD-1]
set_property SEVERITY {Warning} [get_drc_checks UCIO-1]

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets clk_IBUF]

set_property PACKAGE_PIN J4 [get_ports clk]
set_property PACKAGE_PIN L3 [get_ports rst]
set_property PACKAGE_PIN K3 [get_ports updown]

set_property PACKAGE_PIN F15 [get_ports {out[3]}]
set_property PACKAGE_PIN F13 [get_ports {out[2]}]
set_property PACKAGE_PIN F14 [get_ports {out[1]}]
set_property PACKAGE_PIN F16 [get_ports {out[0]}]

set_property PACKAGE_PIN D20 [get_ports {seg[6]}]
set_property PACKAGE_PIN C20 [get_ports {seg[5]}]
set_property PACKAGE_PIN C22 [get_ports {seg[4]}]
set_property PACKAGE_PIN B22 [get_ports {seg[3]}]
set_property PACKAGE_PIN B21 [get_ports {seg[2]}]
set_property PACKAGE_PIN A21 [get_ports {seg[1]}]
set_property PACKAGE_PIN E22 [get_ports {seg[0]}]
set_property PACKAGE_PIN E14 [get_ports digit]


set_property IOSTANDARD LVCMOS18 [get_ports clk]
set_property IOSTANDARD LVCMOS18 [get_ports rst]
set_property IOSTANDARD LVCMOS18 [get_ports updown]
set_property IOSTANDARD LVCMOS18 [get_ports {out[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {out[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {out[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {out[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[6]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[5]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[4]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {seg[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports digit]


