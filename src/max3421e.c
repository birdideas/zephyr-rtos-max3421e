#include <max3421e.h>

static bool host_mode_enabled = false;

int max3421e_read_register(const struct spi_dt_spec spi_dev,
                                  int reg,
                                  uint8_t *const data)
{
    int ret;
    uint8_t command = (uint8_t)(reg << 3) | READ_FLAG;

    /*
     * The spi_buf_set type allows us to specify .count = 2
     * to pass a spi_buf with multiple buffers within itself.
     * Zephyr's stock implementation does this, but here we
     * create fdupspi_data[2] to suppress a compiler warning.
     */

    uint8_t fdupspi_data[2] = {
        0x00, /* hirq */
        0x00  /* data */
    };

    const struct spi_buf command_buf = {
        .buf = &command,
        .len = sizeof(command)
    };

    const struct spi_buf data_buf = {
        .buf = &fdupspi_data,
        .len = sizeof(fdupspi_data)
    };

    const struct spi_buf_set command_buf_set = {
        .buffers = &command_buf,
        .count = 1
    };

    const struct spi_buf_set data_buf_set = {
        .buffers = &data_buf,
        .count = 1
    };

    ret = spi_transceive_dt(&spi_dev, &command_buf_set, &data_buf_set) < 0;
    if (ret) {
        return ret;
    }

    /* Set data manually and ignore hirq. */
    *data = fdupspi_data[1];
    return 0;
}

static int max3421e_safe_write_checks(int reg,
                                     const uint8_t data,
                                     bool host_mode_enabled)
{
    /* The MAX3421E ignores Reg values above 31 */
    if (reg > 31) {
        return -EINVAL;
    }

    /*
     * The CPU should never write the RCVFIFO,
     * because it would corrupt the received data. (p. 49)
     */
    if (reg == RCVFIFO_REGISTER) {
        return -EINVAL;
    }

    /* The CPU should never directly clear the SNDBAVIRQ bit. (p. 56) */
    if (reg == HIRQ_REGISTER && data & SNDBAVIRQ) {
        return -EINVAL;
    }

    /* (p. 7) */
    if (host_mode_enabled) {
        if (reg == 9 || reg == 10 || reg == 11) {
            return -EINVAL;
        }

        if ((reg == USBIRQ_REGISTER && data & VBUSIRQ) ||
            (reg == USBIRQ_REGISTER && data & NOVBUSIRQ) ||
            (reg == USBIEN_REGISTER && data & VBUSIE) ||
            (reg == USBIEN_REGISTER && data & NOVBUSIE))
        {
            return -EINVAL;
        }

        if ((reg == PINCTL_REGISTER  && data & 128) ||
            (reg == PINCTL_REGISTER  && data & 64) ||
            (reg == PINCTL_REGISTER  && data & 32))
        {
            return -EINVAL;
        }
    }

    return 0;
}

int max3421e_write_register(const struct spi_dt_spec spi_dev,
                                   int reg,
                                   const uint8_t data)
{
    const int command = (uint8_t)(reg << 3) | WRITE_FLAG;

    uint8_t tx[2] = { command, data };
    const struct spi_buf buf_tx = { .buf = tx, .len = 2 };
    const struct spi_buf_set buf_tx_set = { .buffers = &buf_tx, .count = 1 };

    if (max3421e_safe_write_checks(reg, data, host_mode_enabled)) {
        return -EINVAL;
    } else {
        return spi_write_dt(&spi_dev, &buf_tx_set);
    }
}

int max3421e_get_revision_number(const struct spi_dt_spec spi_dev,
                                 uint8_t *const data)
{
    return max3421e_read_register(spi_dev, REVISION_REGISTER, data);
}

int max3421e_resolve_revision_number(const struct spi_dt_spec spi_dev,
                                     uint8_t *const data)
{
    int ret, len;
    uint8_t revision_hex;

    static const uint8_t revisions[] = {
        0x01, /* Revision 1 */
        0x12, /* Revision 2 */
        0x13, /* Revision 3 */
    };
    len = sizeof(revisions) / sizeof(uint8_t);

    ret = max3421e_get_revision_number(spi_dev, data);
    if (ret) {
        return ret;
    }

    revision_hex = *data;
    for (int i = 0; i < len; i++) {
        if (revision_hex == revisions[i]) {
            *data = (i + 1);
            return 0;
        }
    }

    /* Unknown revision. Return not supported but leave it in *data. */
    return -ENOTSUP;
}

int max3421e_enable_fdupspi(const struct spi_dt_spec spi_dev)
{
    uint8_t enable_fdupspi_message = FDUPSPI;
    return max3421e_write_register(spi_dev, PINCTL_REGISTER, enable_fdupspi_message);
}

int max3421e_enable_host_mode(const struct spi_dt_spec spi_dev)
{
    uint8_t enable_host_mode_message = DPPULLDN | DMPULLDN | HOST;
    host_mode_enabled = true;
    return max3421e_write_register(spi_dev, MODE_REGISTER, enable_host_mode_message);
}

