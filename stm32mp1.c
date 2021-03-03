#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

typedef struct
{
	uint32_t R[32];			 /*!< 2-step write lock and read back registers,     Address offset: 00h-7Ch  */
	uint32_t RLR[32];		 /*!< 1-step read lock registers,                    Address offset: 80h-FCh  */
	uint32_t C1IER;			 /*!< HSEM Interrupt 0 enable register ,             Address offset: 100h     */
	uint32_t C1ICR;			 /*!< HSEM Interrupt 0 clear register ,              Address offset: 104h     */
	uint32_t C1ISR;			 /*!< HSEM Interrupt 0 Status register ,             Address offset: 108h     */
	uint32_t C1MISR;		 /*!< HSEM Interrupt 0 Masked Status register ,      Address offset: 10Ch     */
	uint32_t C2IER;			 /*!< HSEM Interrupt 1 enable register ,             Address offset: 110h     */
	uint32_t C2ICR;			 /*!< HSEM Interrupt 1 clear register ,              Address offset: 114h     */
	uint32_t C2ISR;			 /*!< HSEM Interrupt 1 Status register ,             Address offset: 118h     */
	uint32_t C2MISR;		 /*!< HSEM Interrupt 1 Masked Status register ,      Address offset: 11Ch     */
	uint32_t Reserved[8];	 /* Reserved                                         Address offset: 120h-13Ch*/
	uint32_t CR;			 /*!< HSEM Semaphore clear register ,                Address offset: 140h      */
	uint32_t KEYR;			 /*!< HSEM Semaphore clear key register ,            Address offset: 144h      */
	uint32_t Reserved1[169]; /* Reserved                                         Address offset: 148h-3E8h */
	uint32_t HWCFGR2;		 /*!< HSEM Hardware Configuration Register 2 ,       Address offset: 3ECh      */
	uint32_t HWCFGR1;		 /*!< HSEM Hardware Configuration Register 1 ,       Address offset: 3F0h      */
	uint32_t VERR;			 /*!< HSEM IP Version Register ,                     Address offset: 3F4h      */
	uint32_t IPIDR;			 /*!< HSEM IP Identification Register ,              Address offset: 3F8h      */
	uint32_t SIDR;			 /*!< HSEM Size Identification Register ,             Address offset: 3FCh      */
} HSEM_TypeDef;

#define HSEM_CR_COREID_Pos (8U)
#define HSEM_CPU1_COREID (0x00000001U) /* Semaphore Core CA7 ID */
#define HSEM_CPU2_COREID (0x00000002U) /* Semaphore Core CM4 ID */
#define HSEM_CR_COREID_CURRENT (HSEM_CPU1_COREID << HSEM_CR_COREID_Pos)
#define HSEM_RLR_LOCK_Pos (31U)
#define HSEM_RLR_LOCK_Msk (0x1UL << HSEM_RLR_LOCK_Pos) /*!< 0x80000000 */
#define HSEM_RLR_LOCK HSEM_RLR_LOCK_Msk				   /*!<Lock indication. */

#define HSEM ((HSEM_TypeDef *)HSEM_BASE)

int main(void)
{
	rcc_periph_clock_enable(RCC_HSEM);
	rcc_periph_clock_enable(RCC_GPIOI);

	while (HSEM->RLR[0U] != (HSEM_CR_COREID_CURRENT | HSEM_RLR_LOCK))
		;

	gpio_mode_setup(GPIOI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);

	HSEM->R[0U] = (0 | HSEM_CR_COREID_CURRENT);

	gpio_clear(GPIOI, GPIO9);

	while (1)
	{
		gpio_toggle(GPIOI, GPIO9);
		for (int i = 0; i < 3200000; i++)
			__asm__("nop");
	}
}
