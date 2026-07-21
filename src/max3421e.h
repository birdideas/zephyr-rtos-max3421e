/* max3421e.h - Brief description of what the library does */

#ifndef MAX3421E_H
#define MAX3421E_H

#include <errno.h>
#include <zephyr/drivers/spi.h>

#define MAX3421E_VERSION_MAJOR 1
#define MAX3421E_VERSION_MINOR 0

#define READ_FLAG (uint8_t) 0 << 1
#define WRITE_FLAG (uint8_t) 1 << 1

/* R1: RCVFIFO */
#define RCVFIFO_REGISTER 1

/* R2: SNDFIFO */
#define SNDFIFO_REGISTER 2

/* R4: SUDFIFO */
#define SUDFIFO_REGISTER 4

/* R6: RCVBC */
#define RCVBC_REGISTER 6

/* R7: SNDBC */
#define SNDBC_REGISTER 7

/* R13: USBIRQ */
#define USBIRQ_REGISTER 13
#define VBUSIRQ   (uint8_t) 1 << 6
#define NOVBUSIRQ (uint8_t) 1 << 5
#define OSCOKIRQ  (uint8_t) 1

/* R14: USBIEN */
#define USBIEN_REGISTER 14
#define VBUSIE   (uint8_t) 1 << 6
#define NOVBUSIE (uint8_t) 1 << 5
#define OSCOKIE  (uint8_t) 1

/* R15: USBCTL */
#define USBCTL_REGISTER 15
#define CHIPRES (uint8_t) 1 << 5
#define PWRDOWN (uint8_t) 1 << 4

/* R16: CPUCTL */
#define PULSEWID1 (uint8_t) 1 << 7
#define PULSEWID0 (uint8_t) 1 << 6
#define IE        (uint8_t) 1

/* R17: PINCTRL */
#define PINCTL_REGISTER 17
#define FDUPSPI  (uint8_t) 1 << 4
#define INTLEVEL (uint8_t) 1 << 3
#define POSINT   (uint8_t) 1 << 2
#define GPXB     (uint8_t) 1 << 1
#define GPXA     (uint8_t) 1

/* R18: REVISION */
#define REVISION_REGISTER 18

/* R20: IOPINS1 */
#define IOPINS1_REGISTER 20
#define GPIN3  (uint8_t) 1 << 7
#define GPIN2  (uint8_t) 1 << 6
#define GPIN1  (uint8_t) 1 << 5
#define GPIN0  (uint8_t) 1 << 4
#define GPOUT3 (uint8_t) 1 << 3
#define GPOUT2 (uint8_t) 1 << 2
#define GPOUT1 (uint8_t) 1 << 1
#define GPOUT0 (uint8_t) 1

/* R21: IOPINS2 */
#define IOPINS2_REGISTER 21
#define GPIN7  (uint8_t) 1 << 7
#define GPIN6  (uint8_t) 1 << 6
#define GPIN5  (uint8_t) 1 << 5
#define GPIN4  (uint8_t) 1 << 4
#define GPOUT7 (uint8_t) 1 << 3
#define GPOUT6 (uint8_t) 1 << 2
#define GPOUT5 (uint8_t) 1 << 1
#define GPOUT4 (uint8_t) 1

/* R22: GPINIRQ */
#define GPINIRQ_REGISTER 22
#define GPINIRQ7 (uint8_t) 1 << 7
#define GPINIRQ6 (uint8_t) 1 << 6
#define GPINIRQ5 (uint8_t) 1 << 5
#define GPINIRQ4 (uint8_t) 1 << 4
#define GPINIRQ3 (uint8_t) 1 << 3
#define GPINIRQ2 (uint8_t) 1 << 2
#define GPINIRQ1 (uint8_t) 1 << 1
#define GPINIRQ0 (uint8_t) 1

/* R23: GPINIEN */
#define GPINIEN_REGISTER 23
#define GPINIEN7 (uint8_t) 1 << 7
#define GPINIEN6 (uint8_t) 1 << 6
#define GPINIEN5 (uint8_t) 1 << 5
#define GPINIEN4 (uint8_t) 1 << 4
#define GPINIEN3 (uint8_t) 1 << 3
#define GPINIEN2 (uint8_t) 1 << 2
#define GPINIEN1 (uint8_t) 1 << 1
#define GPINIEN0 (uint8_t) 1

/* R24: GPINPOL */
#define GPINPOL_REGISTER 24
#define GPINPOL7 (uint8_t) 1 << 7
#define GPINPOL6 (uint8_t) 1 << 6
#define GPINPOL5 (uint8_t) 1 << 5
#define GPINPOL4 (uint8_t) 1 << 4
#define GPINPOL3 (uint8_t) 1 << 3
#define GPINPOL2 (uint8_t) 1 << 2
#define GPINPOL1 (uint8_t) 1 << 1
#define GPINPOL0 (uint8_t) 1

/* R25: HIRQ */
#define HIRQ_REGISTER 25
#define HXFRDNIRQ   (uint8_t) 1 << 7
#define FRAMEIRQ    (uint8_t) 1 << 6
#define CONDETIRQ   (uint8_t) 1 << 5
#define SUSDNIRQ    (uint8_t) 1 << 4
#define SNDBAVIRQ   (uint8_t) 1 << 3
#define RCVDAVIRQ   (uint8_t) 1 << 2
#define RWUIRQ      (uint8_t) 1 << 1
#define BUSEVENTIRQ (uint8_t) 1

/* R26: HIEN */
#define HIEN_REGISTER 26
#define HXFRDNIE   (uint8_t) 1 << 7
#define FRAMEIE    (uint8_t) 1 << 6
#define CONDETIE   (uint8_t) 1 << 5
#define SUSDNIE    (uint8_t) 1 << 4
#define SNDBAVIE   (uint8_t) 1 << 3
#define RCVDAVIE   (uint8_t) 1 << 2
#define RWUIE      (uint8_t) 1 << 1
#define BUSEVENTIE (uint8_t) 1

/* R27: MODE */
#define MODE_REGISTER 27
#define DPPULLDN  (uint8_t) 1 << 7
#define DMPULLDN  (uint8_t) 1 << 6
#define DELAYISO  (uint8_t) 1 << 5
#define SEPIRQ    (uint8_t) 1 << 4
#define SOFKAENAB (uint8_t) 1 << 3
#define HUBPRE    (uint8_t) 1 << 2
#define LOWSPEED  (uint8_t) 1 << 1
#define HOST      (uint8_t) 1

/* R28: PERADDR */
#define PERADDR_REGISTER 28

/* R29: HCTL */
#define HCTL_REGISTER 29
#define SNDTOG1   (uint8_t) 1 << 7
#define SNDTOG0   (uint8_t) 1 << 6
#define RCVTOG1   (uint8_t) 1 << 5
#define RCVTOG0   (uint8_t) 1 << 4
#define SIGRSM    (uint8_t) 1 << 3
#define SAMPLEBUS (uint8_t) 1 << 2
#define FRMRST    (uint8_t) 1 << 1
#define BUSRST    (uint8_t) 1

/* R30: HXFR */
#define HXFR_REGISTER 30
#define HS     (uint8_t) 1 << 7
#define ISO    (uint8_t) 1 << 6
#define OUTNIN (uint8_t) 1 << 5
#define SETUP  (uint8_t) 1 << 4
#define EP3    (uint8_t) 1 << 3
#define EP2    (uint8_t) 1 << 2
#define EP1    (uint8_t) 1 << 1
#define EP0    (uint8_t) 1

/* R31: HRSL */
#define HRSL_REGISTER 31
#define JSTATUS  (uint8_t) 1 << 7
#define KSTATUS  (uint8_t) 1 << 6
#define SNDTOGRD (uint8_t) 1 << 5
#define RCVTOGRD (uint8_t) 1 << 4
#define HRSLT3   (uint8_t) 1 << 3
#define HRSLT2   (uint8_t) 1 << 2
#define HRSLT1   (uint8_t) 1 << 1
#define HRSTL0   (uint8_t) 1

static const struct spi_dt_spec spi_dev;
int max3421e_enable_fdupspi(const struct spi_dt_spec spi_dev);
int max3421e_get_revision_number(const struct spi_dt_spec spi_dev, uint8_t *const data);
int max3421e_resolve_revision_number(const struct spi_dt_spec spi_dev, uint8_t *const data);
int max3421e_enable_host_mode(const struct spi_dt_spec spi_dev);
int max3421e_read_register(const struct spi_dt_spec spi_dev, int reg, uint8_t *const data);
int max3421e_write_register(const struct spi_dt_spec spi_dev, int reg, uint8_t const data);

#endif /* MAX3421E_H */
