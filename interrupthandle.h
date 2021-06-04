#ifndef __interrupthandle_H__
#define __interrupthandle_H__

#include "types.h"
#include "color.h"
#include "gdt.h"
#include "port.h"

class InterruptHandler {

	protected:
		struct GateDescriptor {
		
			uint16_t handleAddressLowBits;
			uint16_t gdt_codeSegmentSelector;
			uint8_t reserved;
			uint8_t access;
			uint16_t handleAddressHighBits;

		} __attribute__((packed));

		static GateDescriptor interruptDescriptorTable[256];

		struct interruptDescripterTablePointer
		{
			uint16_t size;
			uint32_t base;
		} __attribute__((packed));
		
		static void SetInterruptDescriptorTableEntry(
				uint8_t interruptnumber,
				uint16_t codeSegmentSelectorOffset,
				void (*handler)(),
				uint8_t PriviledgeLevel,
				uint8_t Type);
		
		Port8bitSlow MasterCommand;
		Port8bitSlow Masterdata;


		Port8bitSlow SlaveCommand;
		Port8bitSlow Slavedata;

	public:
		static uint32_t HandleInterrupt(uint8_t interruptnumber, uint32_t esp);

		static void IgnoreInterruptRequest();
		static void HandleInterruptRequest0x00();
		static void HandleInterruptRequest0x01();

		InterruptHandler(GlobalDescriptorTable* gdt);
		~InterruptHandler();

		void Activate();
};


#endif
