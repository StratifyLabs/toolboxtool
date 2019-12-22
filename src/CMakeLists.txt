

sos_sdk_add_subdirectory(SOURCE_LIST test)

list(
	APPEND SOURCE_LIST
	sl_config.h
	main.cpp
	)


set(SOURCES ${SOURCE_LIST} PARENT_SCOPE)
