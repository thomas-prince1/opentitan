// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
// clang-format off
//
// ------------------- W A R N I N G: A U T O - G E N E R A T E D   C O D E !! -------------------//
// PLEASE DO NOT HAND-EDIT THIS FILE. IT HAS BEEN AUTO-GENERATED WITH THE FOLLOWING COMMAND:
// util/topgen.py -t hw/top_englishbreakfast/data/top_englishbreakfast.hjson
// -o hw/top_englishbreakfast
#include <limits.h>

#include "sw/device/lib/base/csr.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_aon_timer.h"
#include "sw/device/lib/dif/dif_flash_ctrl.h"
#include "sw/device/lib/dif/dif_gpio.h"
#include "sw/device/lib/dif/dif_hmac.h"
#include "sw/device/lib/dif/dif_pwrmgr.h"
#include "sw/device/lib/dif/dif_rv_plic.h"
#include "sw/device/lib/dif/dif_spi_device.h"
#include "sw/device/lib/dif/dif_spi_host.h"
#include "sw/device/lib/dif/dif_uart.h"
#include "sw/device/lib/dif/dif_usbdev.h"
#include "sw/device/lib/runtime/irq.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/rv_plic_testutils.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"
#include "sw/device/lib/testing/test_framework/status.h"
#include "hw/top_englishbreakfast/sw/autogen/top_englishbreakfast.h"

static dif_aon_timer_t aon_timer_aon;
static dif_flash_ctrl_t flash_ctrl;
static dif_gpio_t gpio;
static dif_hmac_t hmac;
static dif_pwrmgr_t pwrmgr_aon;
static dif_spi_device_t spi_device;
static dif_spi_host_t spi_host0;
static dif_uart_t uart0;
static dif_usbdev_t usbdev;
static dif_rv_plic_t plic;
static const top_englishbreakfast_plic_target_t kHart = kTopEnglishbreakfastPlicTargetIbex0;

/**
 * Flag indicating which peripheral is under test.
 *
 * Declared volatile because it is referenced in the main program flow as well
 * as the ISR.
 */
static volatile top_englishbreakfast_plic_peripheral_t peripheral_expected;

/**
 * Flags indicating the IRQ expected to have triggered and serviced within the
 * peripheral.
 *
 * Declared volatile because it is referenced in the main program flow as well
 * as the ISR.
 */
static volatile dif_aon_timer_irq_t aon_timer_irq_expected;
static volatile dif_aon_timer_irq_t aon_timer_irq_serviced;
static volatile dif_flash_ctrl_irq_t flash_ctrl_irq_expected;
static volatile dif_flash_ctrl_irq_t flash_ctrl_irq_serviced;
static volatile dif_gpio_irq_t gpio_irq_expected;
static volatile dif_gpio_irq_t gpio_irq_serviced;
static volatile dif_hmac_irq_t hmac_irq_expected;
static volatile dif_hmac_irq_t hmac_irq_serviced;
static volatile dif_pwrmgr_irq_t pwrmgr_irq_expected;
static volatile dif_pwrmgr_irq_t pwrmgr_irq_serviced;
static volatile dif_spi_device_irq_t spi_device_irq_expected;
static volatile dif_spi_device_irq_t spi_device_irq_serviced;
static volatile dif_spi_host_irq_t spi_host_irq_expected;
static volatile dif_spi_host_irq_t spi_host_irq_serviced;
static volatile dif_uart_irq_t uart_irq_expected;
static volatile dif_uart_irq_t uart_irq_serviced;
static volatile dif_usbdev_irq_t usbdev_irq_expected;
static volatile dif_usbdev_irq_t usbdev_irq_serviced;

/**
 * Provides external IRQ handling for this test.
 *
 * This function overrides the default OTTF external ISR.
 *
 * For each IRQ, it performs the following:
 * 1. Claims the IRQ fired (finds PLIC IRQ index).
 * 2. Checks that the index belongs to the expected peripheral.
 * 3. Checks that the correct and the only IRQ from the expected peripheral
 *    triggered.
 * 4. Clears the IRQ at the peripheral.
 * 5. Completes the IRQ service at PLIC.
 */
void ottf_external_isr(void) {
  dif_rv_plic_irq_id_t plic_irq_id;
  CHECK_DIF_OK(dif_rv_plic_irq_claim(&plic, kHart, &plic_irq_id));

  top_englishbreakfast_plic_peripheral_t peripheral = (top_englishbreakfast_plic_peripheral_t)
      top_englishbreakfast_plic_interrupt_for_peripheral[plic_irq_id];
  CHECK(peripheral == peripheral_expected,
        "Interrupt from incorrect peripheral: exp = %d, obs = %d",
        peripheral_expected, peripheral);

  switch (peripheral) {
    case kTopEnglishbreakfastPlicPeripheralAonTimerAon: {
      dif_aon_timer_irq_t irq = (dif_aon_timer_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdAonTimerAonWkupTimerExpired);
      CHECK(irq == aon_timer_irq_expected,
            "Incorrect aon_timer_aon IRQ triggered: exp = %d, obs = %d",
            aon_timer_irq_expected, irq);
      aon_timer_irq_serviced = irq;

      dif_aon_timer_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_aon_timer_irq_get_state(&aon_timer_aon, &snapshot));
      CHECK(snapshot == (dif_aon_timer_irq_state_snapshot_t)(1 << irq),
            "Only aon_timer_aon IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_aon_timer_irq_force(&aon_timer_aon, irq, false));
      CHECK_DIF_OK(dif_aon_timer_irq_acknowledge(&aon_timer_aon, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralFlashCtrl: {
      dif_flash_ctrl_irq_t irq = (dif_flash_ctrl_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdFlashCtrlProgEmpty);
      CHECK(irq == flash_ctrl_irq_expected,
            "Incorrect flash_ctrl IRQ triggered: exp = %d, obs = %d",
            flash_ctrl_irq_expected, irq);
      flash_ctrl_irq_serviced = irq;

      dif_flash_ctrl_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_flash_ctrl_irq_get_state(&flash_ctrl, &snapshot));
      CHECK(snapshot == (dif_flash_ctrl_irq_state_snapshot_t)(1 << irq),
            "Only flash_ctrl IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_flash_ctrl_irq_force(&flash_ctrl, irq, false));
      CHECK_DIF_OK(dif_flash_ctrl_irq_acknowledge(&flash_ctrl, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralGpio: {
      dif_gpio_irq_t irq = (dif_gpio_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdGpioGpio0);
      CHECK(irq == gpio_irq_expected,
            "Incorrect gpio IRQ triggered: exp = %d, obs = %d",
            gpio_irq_expected, irq);
      gpio_irq_serviced = irq;

      dif_gpio_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_gpio_irq_get_state(&gpio, &snapshot));
      CHECK(snapshot == (dif_gpio_irq_state_snapshot_t)(1 << irq),
            "Only gpio IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_gpio_irq_force(&gpio, irq, false));
      CHECK_DIF_OK(dif_gpio_irq_acknowledge(&gpio, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralHmac: {
      dif_hmac_irq_t irq = (dif_hmac_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdHmacHmacDone);
      CHECK(irq == hmac_irq_expected,
            "Incorrect hmac IRQ triggered: exp = %d, obs = %d",
            hmac_irq_expected, irq);
      hmac_irq_serviced = irq;

      dif_hmac_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_hmac_irq_get_state(&hmac, &snapshot));
      CHECK(snapshot == (dif_hmac_irq_state_snapshot_t)(1 << irq),
            "Only hmac IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_hmac_irq_force(&hmac, irq, false));
      CHECK_DIF_OK(dif_hmac_irq_acknowledge(&hmac, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralPwrmgrAon: {
      dif_pwrmgr_irq_t irq = (dif_pwrmgr_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdPwrmgrAonWakeup);
      CHECK(irq == pwrmgr_irq_expected,
            "Incorrect pwrmgr_aon IRQ triggered: exp = %d, obs = %d",
            pwrmgr_irq_expected, irq);
      pwrmgr_irq_serviced = irq;

      dif_pwrmgr_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_pwrmgr_irq_get_state(&pwrmgr_aon, &snapshot));
      CHECK(snapshot == (dif_pwrmgr_irq_state_snapshot_t)(1 << irq),
            "Only pwrmgr_aon IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_pwrmgr_irq_force(&pwrmgr_aon, irq, false));
      CHECK_DIF_OK(dif_pwrmgr_irq_acknowledge(&pwrmgr_aon, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralSpiDevice: {
      dif_spi_device_irq_t irq = (dif_spi_device_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdSpiDeviceGenericRxFull);
      CHECK(irq == spi_device_irq_expected,
            "Incorrect spi_device IRQ triggered: exp = %d, obs = %d",
            spi_device_irq_expected, irq);
      spi_device_irq_serviced = irq;

      dif_spi_device_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_spi_device_irq_get_state(&spi_device, &snapshot));
      CHECK(snapshot == (dif_spi_device_irq_state_snapshot_t)(1 << irq),
            "Only spi_device IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_spi_device_irq_force(&spi_device, irq, false));
      CHECK_DIF_OK(dif_spi_device_irq_acknowledge(&spi_device, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralSpiHost0: {
      dif_spi_host_irq_t irq = (dif_spi_host_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdSpiHost0Error);
      CHECK(irq == spi_host_irq_expected,
            "Incorrect spi_host0 IRQ triggered: exp = %d, obs = %d",
            spi_host_irq_expected, irq);
      spi_host_irq_serviced = irq;

      dif_spi_host_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_spi_host_irq_get_state(&spi_host0, &snapshot));
      CHECK(snapshot == (dif_spi_host_irq_state_snapshot_t)(1 << irq),
            "Only spi_host0 IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_spi_host_irq_force(&spi_host0, irq, false));
      CHECK_DIF_OK(dif_spi_host_irq_acknowledge(&spi_host0, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralUart0: {
      dif_uart_irq_t irq = (dif_uart_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdUart0TxWatermark);
      CHECK(irq == uart_irq_expected,
            "Incorrect uart0 IRQ triggered: exp = %d, obs = %d",
            uart_irq_expected, irq);
      uart_irq_serviced = irq;

      dif_uart_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_uart_irq_get_state(&uart0, &snapshot));
      CHECK(snapshot == (dif_uart_irq_state_snapshot_t)(1 << irq),
            "Only uart0 IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_uart_irq_force(&uart0, irq, false));
      CHECK_DIF_OK(dif_uart_irq_acknowledge(&uart0, irq));
      break;
    }

    case kTopEnglishbreakfastPlicPeripheralUsbdev: {
      dif_usbdev_irq_t irq = (dif_usbdev_irq_t)(
          plic_irq_id -
          (dif_rv_plic_irq_id_t)kTopEnglishbreakfastPlicIrqIdUsbdevPktReceived);
      CHECK(irq == usbdev_irq_expected,
            "Incorrect usbdev IRQ triggered: exp = %d, obs = %d",
            usbdev_irq_expected, irq);
      usbdev_irq_serviced = irq;

      dif_usbdev_irq_state_snapshot_t snapshot;
      CHECK_DIF_OK(dif_usbdev_irq_get_state(&usbdev, &snapshot));
      CHECK(snapshot == (dif_usbdev_irq_state_snapshot_t)(1 << irq),
            "Only usbdev IRQ %d expected to fire. Actual interrupt "
            "status = %x",
            irq, snapshot);

      // TODO: Check Interrupt type then clear INTR_TEST if needed.
      CHECK_DIF_OK(dif_usbdev_irq_force(&usbdev, irq, false));
      CHECK_DIF_OK(dif_usbdev_irq_acknowledge(&usbdev, irq));
      break;
    }

    default:
      LOG_FATAL("ISR is not implemented!");
      test_status_set(kTestStatusFailed);
  }

  // Complete the IRQ at PLIC.
  CHECK_DIF_OK(dif_rv_plic_irq_complete(&plic, kHart, plic_irq_id));
}

/**
 * Initializes the handles to all peripherals.
 */
static void peripherals_init(void) {
  mmio_region_t base_addr;

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_AON_TIMER_AON_BASE_ADDR);
  CHECK_DIF_OK(dif_aon_timer_init(base_addr, &aon_timer_aon));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_FLASH_CTRL_CORE_BASE_ADDR);
  CHECK_DIF_OK(dif_flash_ctrl_init(base_addr, &flash_ctrl));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_GPIO_BASE_ADDR);
  CHECK_DIF_OK(dif_gpio_init(base_addr, &gpio));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_HMAC_BASE_ADDR);
  CHECK_DIF_OK(dif_hmac_init(base_addr, &hmac));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_PWRMGR_AON_BASE_ADDR);
  CHECK_DIF_OK(dif_pwrmgr_init(base_addr, &pwrmgr_aon));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_SPI_DEVICE_BASE_ADDR);
  CHECK_DIF_OK(dif_spi_device_init(base_addr, &spi_device));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_SPI_HOST0_BASE_ADDR);
  CHECK_DIF_OK(dif_spi_host_init(base_addr, &spi_host0));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_UART0_BASE_ADDR);
  CHECK_DIF_OK(dif_uart_init(base_addr, &uart0));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_USBDEV_BASE_ADDR);
  CHECK_DIF_OK(dif_usbdev_init(base_addr, &usbdev));

  base_addr = mmio_region_from_addr(TOP_ENGLISHBREAKFAST_RV_PLIC_BASE_ADDR);
  CHECK_DIF_OK(dif_rv_plic_init(base_addr, &plic));
}

/**
 * Clears pending IRQs in all peripherals.
 */
static void peripheral_irqs_clear(void) {
  CHECK_DIF_OK(dif_aon_timer_irq_acknowledge_all(&aon_timer_aon));
  CHECK_DIF_OK(dif_flash_ctrl_irq_acknowledge_all(&flash_ctrl));
  CHECK_DIF_OK(dif_gpio_irq_acknowledge_all(&gpio));
  CHECK_DIF_OK(dif_hmac_irq_acknowledge_all(&hmac));
  CHECK_DIF_OK(dif_pwrmgr_irq_acknowledge_all(&pwrmgr_aon));
  CHECK_DIF_OK(dif_spi_device_irq_acknowledge_all(&spi_device));
  CHECK_DIF_OK(dif_spi_host_irq_acknowledge_all(&spi_host0));
  CHECK_DIF_OK(dif_uart_irq_acknowledge_all(&uart0));
  CHECK_DIF_OK(dif_usbdev_irq_acknowledge_all(&usbdev));
}

/**
 * Enables all IRQs in all peripherals.
 */
static void peripheral_irqs_enable(void) {
  dif_flash_ctrl_irq_state_snapshot_t flash_ctrl_irqs =
      (dif_flash_ctrl_irq_state_snapshot_t)UINT_MAX;
  dif_gpio_irq_state_snapshot_t gpio_irqs =
      (dif_gpio_irq_state_snapshot_t)UINT_MAX;
  dif_hmac_irq_state_snapshot_t hmac_irqs =
      (dif_hmac_irq_state_snapshot_t)UINT_MAX;
  dif_pwrmgr_irq_state_snapshot_t pwrmgr_irqs =
      (dif_pwrmgr_irq_state_snapshot_t)UINT_MAX;
  dif_spi_device_irq_state_snapshot_t spi_device_irqs =
      (dif_spi_device_irq_state_snapshot_t)UINT_MAX;
  dif_spi_host_irq_state_snapshot_t spi_host_irqs =
      (dif_spi_host_irq_state_snapshot_t)UINT_MAX;
  dif_uart_irq_state_snapshot_t uart_irqs =
      (dif_uart_irq_state_snapshot_t)UINT_MAX;
  dif_usbdev_irq_state_snapshot_t usbdev_irqs =
      (dif_usbdev_irq_state_snapshot_t)UINT_MAX;

  CHECK_DIF_OK(
      dif_flash_ctrl_irq_restore_all(&flash_ctrl, &flash_ctrl_irqs));
  CHECK_DIF_OK(
      dif_gpio_irq_restore_all(&gpio, &gpio_irqs));
  CHECK_DIF_OK(
      dif_hmac_irq_restore_all(&hmac, &hmac_irqs));
  CHECK_DIF_OK(
      dif_pwrmgr_irq_restore_all(&pwrmgr_aon, &pwrmgr_irqs));
  CHECK_DIF_OK(
      dif_spi_device_irq_restore_all(&spi_device, &spi_device_irqs));
  CHECK_DIF_OK(
      dif_spi_host_irq_restore_all(&spi_host0, &spi_host_irqs));
  // lowrisc/opentitan#8656: Skip UART0 in non-DV setups due to interference
  // from the logging facility.
  if (kDeviceType == kDeviceSimDV) {
    CHECK_DIF_OK(
        dif_uart_irq_restore_all(&uart0, &uart_irqs));
  }
  CHECK_DIF_OK(
      dif_usbdev_irq_restore_all(&usbdev, &usbdev_irqs));
}

/**
 * Triggers all IRQs in all peripherals one by one.
 *
 * Walks through all instances of all peripherals and triggers an interrupt one
 * by one, by forcing with the `intr_test` CSR. On trigger, the CPU instantly
 * jumps into the ISR. The main flow of execution thus proceeds to check that
 * the correct IRQ was serviced immediately. The ISR, in turn checks if the
 * expected IRQ from the expected peripheral triggered.
 */
static void peripheral_irqs_trigger(void) {
  peripheral_expected = kTopEnglishbreakfastPlicPeripheralAonTimerAon;
  for (dif_aon_timer_irq_t irq = kDifAonTimerIrqWkupTimerExpired;
       irq <= kDifAonTimerIrqWdogTimerBark; ++irq) {
    aon_timer_irq_expected = irq;
    LOG_INFO("Triggering aon_timer_aon IRQ %d.", irq);
    CHECK_DIF_OK(dif_aon_timer_irq_force(&aon_timer_aon, irq, true));

    // TODO: Make race-condition free
    CHECK(aon_timer_irq_serviced == irq,
          "Incorrect aon_timer_aon IRQ serviced: exp = %d, obs = %d", irq,
          aon_timer_irq_serviced);
    LOG_INFO("IRQ %d from aon_timer_aon is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralFlashCtrl;
  for (dif_flash_ctrl_irq_t irq = kDifFlashCtrlIrqProgEmpty;
       irq <= kDifFlashCtrlIrqCorrErr; ++irq) {
    flash_ctrl_irq_expected = irq;
    LOG_INFO("Triggering flash_ctrl IRQ %d.", irq);
    CHECK_DIF_OK(dif_flash_ctrl_irq_force(&flash_ctrl, irq, true));

    // TODO: Make race-condition free
    CHECK(flash_ctrl_irq_serviced == irq,
          "Incorrect flash_ctrl IRQ serviced: exp = %d, obs = %d", irq,
          flash_ctrl_irq_serviced);
    LOG_INFO("IRQ %d from flash_ctrl is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralGpio;
  for (dif_gpio_irq_t irq = kDifGpioIrqGpio0;
       irq <= kDifGpioIrqGpio31; ++irq) {
    gpio_irq_expected = irq;
    LOG_INFO("Triggering gpio IRQ %d.", irq);
    CHECK_DIF_OK(dif_gpio_irq_force(&gpio, irq, true));

    // TODO: Make race-condition free
    CHECK(gpio_irq_serviced == irq,
          "Incorrect gpio IRQ serviced: exp = %d, obs = %d", irq,
          gpio_irq_serviced);
    LOG_INFO("IRQ %d from gpio is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralHmac;
  for (dif_hmac_irq_t irq = kDifHmacIrqHmacDone;
       irq <= kDifHmacIrqHmacErr; ++irq) {
    hmac_irq_expected = irq;
    LOG_INFO("Triggering hmac IRQ %d.", irq);
    CHECK_DIF_OK(dif_hmac_irq_force(&hmac, irq, true));

    // TODO: Make race-condition free
    CHECK(hmac_irq_serviced == irq,
          "Incorrect hmac IRQ serviced: exp = %d, obs = %d", irq,
          hmac_irq_serviced);
    LOG_INFO("IRQ %d from hmac is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralPwrmgrAon;
  for (dif_pwrmgr_irq_t irq = kDifPwrmgrIrqWakeup;
       irq <= kDifPwrmgrIrqWakeup; ++irq) {
    pwrmgr_irq_expected = irq;
    LOG_INFO("Triggering pwrmgr_aon IRQ %d.", irq);
    CHECK_DIF_OK(dif_pwrmgr_irq_force(&pwrmgr_aon, irq, true));

    // TODO: Make race-condition free
    CHECK(pwrmgr_irq_serviced == irq,
          "Incorrect pwrmgr_aon IRQ serviced: exp = %d, obs = %d", irq,
          pwrmgr_irq_serviced);
    LOG_INFO("IRQ %d from pwrmgr_aon is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralSpiDevice;
  for (dif_spi_device_irq_t irq = kDifSpiDeviceIrqGenericRxFull;
       irq <= kDifSpiDeviceIrqTpmHeaderNotEmpty; ++irq) {
    spi_device_irq_expected = irq;
    LOG_INFO("Triggering spi_device IRQ %d.", irq);
    CHECK_DIF_OK(dif_spi_device_irq_force(&spi_device, irq, true));

    // TODO: Make race-condition free
    CHECK(spi_device_irq_serviced == irq,
          "Incorrect spi_device IRQ serviced: exp = %d, obs = %d", irq,
          spi_device_irq_serviced);
    LOG_INFO("IRQ %d from spi_device is serviced.", irq);
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralSpiHost0;
  for (dif_spi_host_irq_t irq = kDifSpiHostIrqError;
       irq <= kDifSpiHostIrqSpiEvent; ++irq) {
    spi_host_irq_expected = irq;
    LOG_INFO("Triggering spi_host0 IRQ %d.", irq);
    CHECK_DIF_OK(dif_spi_host_irq_force(&spi_host0, irq, true));

    // TODO: Make race-condition free
    CHECK(spi_host_irq_serviced == irq,
          "Incorrect spi_host0 IRQ serviced: exp = %d, obs = %d", irq,
          spi_host_irq_serviced);
    LOG_INFO("IRQ %d from spi_host0 is serviced.", irq);
  }

  // lowrisc/opentitan#8656: Skip UART0 in non-DV setups due to interference
  // from the logging facility.
  if (kDeviceType == kDeviceSimDV) {
    peripheral_expected = kTopEnglishbreakfastPlicPeripheralUart0;
    for (dif_uart_irq_t irq = kDifUartIrqTxWatermark;
         irq <= kDifUartIrqRxParityErr; ++irq) {
      uart_irq_expected = irq;
      LOG_INFO("Triggering uart0 IRQ %d.", irq);
      CHECK_DIF_OK(dif_uart_irq_force(&uart0, irq, true));

      // TODO: Make race-condition free
      CHECK(uart_irq_serviced == irq,
            "Incorrect uart0 IRQ serviced: exp = %d, obs = %d", irq,
            uart_irq_serviced);
      LOG_INFO("IRQ %d from uart0 is serviced.", irq);
    }
  }

  peripheral_expected = kTopEnglishbreakfastPlicPeripheralUsbdev;
  for (dif_usbdev_irq_t irq = kDifUsbdevIrqPktReceived;
       irq <= kDifUsbdevIrqLinkOutErr; ++irq) {
    usbdev_irq_expected = irq;
    LOG_INFO("Triggering usbdev IRQ %d.", irq);
    CHECK_DIF_OK(dif_usbdev_irq_force(&usbdev, irq, true));

    // TODO: Make race-condition free
    CHECK(usbdev_irq_serviced == irq,
          "Incorrect usbdev IRQ serviced: exp = %d, obs = %d", irq,
          usbdev_irq_serviced);
    LOG_INFO("IRQ %d from usbdev is serviced.", irq);
  }
}

/**
 * Checks that the target ID corresponds to the ID of the hart on which
 * this test is executed on. This check is meant to be used in a
 * single-hart system only.
 */
static void check_hart_id(uint32_t exp_hart_id) {
  uint32_t act_hart_id;
  CSR_READ(CSR_REG_MHARTID, &act_hart_id);
  CHECK(act_hart_id == exp_hart_id, "Processor has unexpected HART ID.");
}

OTTF_DEFINE_TEST_CONFIG();

bool test_main(void) {
  irq_global_ctrl(true);
  irq_external_ctrl(true);
  peripherals_init();
  check_hart_id((uint32_t)kHart);
  rv_plic_testutils_irq_range_enable(
      &plic, kHart, kTopEnglishbreakfastPlicIrqIdNone + 1, kTopEnglishbreakfastPlicIrqIdLast);
  peripheral_irqs_clear();
  peripheral_irqs_enable();
  peripheral_irqs_trigger();
  return true;
}

// clang-format on
