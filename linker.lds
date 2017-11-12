
OUTPUT_ARCH( "riscv" )
ENTRY( reset_vector )

MEMORY {
	ROM (rx):  ORIGIN = 0x80000000, LENGTH = 0x8000
	RAM (rwx): ORIGIN = 0x80100000, LENGTH = 0x8000
}

SECTIONS
{

  /*--------------------------------------------------------------------*/
  /* Code and read-only segment                                         */
  /*--------------------------------------------------------------------*/

  /* Begining of code and text segment */
  . = 0x80000000;
  _ftext = .;
  PROVIDE( eprol = . );

  .text :
  {
    *(.text.init)
  } >ROM

  /* text: Program code section */
  .text : 
  {
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t.*)
  } >ROM

  /* rodata: Read-only data */
  .rodata : 
  {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
  } >ROM

  /* End of code and read-only segment */
  PROVIDE( etext = . );
  _etext = .;

  . = 0x80100000;

  /*--------------------------------------------------------------------*/
  /* Initialized data segment                                           */
  /*--------------------------------------------------------------------*/
  /* data: Writable data */
  .data : 
  {
    __DATA_START__ = .;
    *(.data)
    *(.data*)
    *(.srodata*)
    *(.gnu.linkonce.d.*)
    *(.comment)
    __DATA_END__ = .;
  } >RAM AT>ROM

  .bss : 
  {
    __BSS_START__ = .;
    *(.bss)
    *(.bss.*)
    *(.sbss*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    __BSS_END__ = .;
  } >RAM AT>ROM

  __DATA_LMA_START__ = LOADADDR(.data);
  __DATA_SIZE__ = SIZEOF(.data);

  __BSS_LMA_START__ = LOADADDR(.bss);
  __BSS_SIZE__ = SIZEOF(.bss);


  _end = .;
}
