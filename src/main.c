#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include <max3421e.h>

LOG_MODULE_REGISTER(maxtest, LOG_LEVEL_INF);

static const struct spi_dt_spec spi_dev = SPI_DT_SPEC_GET(
    DT_NODELABEL(max3421e),
    SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB
);

int main(void)
{
    int ret;
    uint8_t data;

    if (!spi_is_ready_dt(&spi_dev)) {
        LOG_ERR("Error initializing SPI device.");
        return -ENODEV;
    } else {
        LOG_INF("Successfully initialized SPI device.");
    }

    ret = max3421e_enable_fdupspi(spi_dev);
    if (ret) {
        LOG_ERR("Error enabling fdupspi: %d", ret);
        return -1;
    }

    max3421e_read_register(spi_dev, PINCTL_REGISTER, &data);
    if (data == FDUPSPI) {
        LOG_INF("Successfully initialized FDUPSPI.");
    }

    ret = max3421e_resolve_revision_number(spi_dev, &data);
    if (ret) {
        LOG_ERR("Error resolving revision number: %d", ret);
        return -1;
    } else {
        LOG_INF("Successfully read revision number");
    }

    ret = max3421e_enable_host_mode(spi_dev);
    if (ret) {
        LOG_ERR("Error enabling host mode: %d", ret);
    }

    max3421e_read_register(spi_dev, MODE_REGISTER, &data);
    if (data == (DPPULLDN | DMPULLDN | HOST)) {
        LOG_INF("Successfully enabled host mode.");
    } else if (data == (DPPULLDN | DMPULLDN)) {
        LOG_ERR("Could not latch HOST bit. Check your reset pin.");
    } else {
        LOG_ERR("Could not write to MODE register.");
    }

    return 0;
}

