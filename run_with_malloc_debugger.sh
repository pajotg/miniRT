#make debug

~/custom_commands/inject ~/injections/malloc.so ./miniRT $@ 2> temp
~/custom_commands/addr2line_replace temp ./miniRT > output
rm temp

#./run_with_malloc_debugger.sh MALLOC_NULL=42 PRINT_MALLOCS_START=32 PRINT_MALLOCS_END=52 scenes/valid/everything.rt