px4_add_module(
	MODULE modules__rw_uart
	MAIN rw_uart
	COMPILE_FLAGS
		-Os
	SRCS
		rw_uart.c
	DEPENDS
	#	platforms_common
)
