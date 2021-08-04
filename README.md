# libstcrypto-example-nucleo_l552ze-q
This is an example project demonstrating libstcrypto usage on ©STMicroelectronics' target NUCLEO L552ZE-Q.

More info on the development platform: \
https://www.st.com/en/evaluation-tools/nucleo-l552ze-q.html

## License information
Majority of the source code is using MIT License - mainly code written by the author.
Please see LICENSE file for more details.

Some of the parts might be using SLA0044 license. See file below for more details: \
https://www.st.com/content/ccc/resource/legal/legal_agreement/license_agreement/group0/87/0c/3d/ad/0a/ba/44/26/DM00216740/files/DM00216740.pdf/jcr:content/translations/en.DM00216740.pdf

## Project dependencies
- **libstcrypto**: \
https://github.com/sgrams/libstcrypto \
A library providing lightweight cryptography algorithms for Arm® Cortex-M microprocessors.

- **STM32CubeIDE**: \
https://www.st.com/en/development-tools/stm32cubeide.html \
Used as the main project's C toolchain (based on GNU GCC Arm®) as well as code generator.

- **STM32CubeProg**: \
https://www.st.com/en/development-tools/stm32cubeprog.html \
Used as an additional software for option bytes programming and more detailed platform configuration tool.

- **GNU Arm Embedded Toolchain**: \
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
Pre-built toolchain for target's Arm® Cortex®-M33 core.

## Option bytes
In order to program the target, following option bytes **MUST** be set: \
- `TZEN = 1` – marking TZ (Arm® TrustZone) enabled
- `DBANK = 1` – marking flash dual banking enabled 
- `SECWM1_PSTRT = 0x00`, `SECWM1_PEND = 0x7F` – marking all 128 pages of Bank1 flash secured
- `SECMW2_PSTRT = 0x01`, `SECWM2_PEND = 0x00` – marking none pages of Bank2 flash secured 

## Flash layout
The flash of the target (512 KB) is split into two halves (each 256 KB sized) for secure and non-secure domains. \
TBC

## Debug messages
All debug messages use SWV ITM (Serial Wire Viewer Instruction Trace Module). \
This is achieved by using `__io_putchar` function overloading (weak attribute). \
See `(Secure|NonSecure)/Core/Src/syscall.c` for the actual implementation.

## Example usage
This example project provides following use cases of **libstcrypto** library:
- TBC
- TBC

---
Stanislaw Grams ® <stanislawgrams@gmail.com>
