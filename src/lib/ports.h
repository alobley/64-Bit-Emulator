#ifndef PORTS_H
#define PORTS_H

#define PORT_KBD_READ 0x01      // Read data from the keyboard

#define PORT_MOUSE 0x02         // Mouse port. Likely won't be used for the forseeable future.

#define PORT_PIT_GET 0x03       // Check for a PIT tick
#define PORT_PIT_SET 0x04       // Set the frequency of the PIT

#define PORT_PIC_FUNCTION 0x05  // Set the destination of what will be written (for example pointer, interrupt number)
#define PORT_PIC_WRITE 0x06     // Write to the destination pointed to by the PIC function register
#define PORT_PIC_READ 0x07      // Read a PIC register

#define PORT_PCI 0x00           // Unimplemented

#endif