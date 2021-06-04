#include "interrupthandle.h"

void print(const char *, int, int);

uint32_t InterruptHandler::HandleInterrupt(uint8_t interruptnumber, uint32_t esp)
{
	print("INTERRUPT RECIEVED\n", BLACK, RED);
	return esp;
}

InterruptHandler::GateDescriptor InterruptHandler::interruptDescriptorTable[256];

void InterruptHandler::SetInterruptDescriptorTableEntry(
		uint8_t interruptnumber,
		uint16_t codeSegmentSelectorOffset,
		void (*handler)(),
		uint8_t PriviledgeLevel,
		uint8_t Type)
{
	const uint8_t IDT_D_PRESENT = 0x80;

	interruptDescriptorTable[interruptnumber].handleAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptnumber].handleAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptnumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptnumber].access = IDT_D_PRESENT | Type | ((PriviledgeLevel & 3) << 5);
	interruptDescriptorTable[interruptnumber].reserved = 0;
}



InterruptHandler::~InterruptHandler() {}

InterruptHandler::InterruptHandler(GlobalDescriptorTable* gdt)
	: MasterCommand(0x20),
	Masterdata(0x21),
	SlaveCommand(0xA0),
	Slavedata(0xA1)
{
	uint16_t CodeSegment = gdt -> CodeSegmentChooser();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	for(uint16_t i = 0; i < 256; i++)
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);

	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	MasterCommand.Write(0x11);
	SlaveCommand.Write(0x11);

	Masterdata.Write(0x20);
	Slavedata.Write(0x28);
	
	Masterdata.Write(0x04);
	Slavedata.Write(0x02);
	
	Masterdata.Write(0x01);
	Slavedata.Write(0x01);

	Masterdata.Write(0x00);
	Slavedata.Write(0x00);

	interruptDescripterTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;

	asm volatile("lidt %0": : "m" (idt));

}


void InterruptHandler::Activate()
{
	asm("sti");
}
