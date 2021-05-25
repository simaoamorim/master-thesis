#include "comm.h"

int comm_get_inputs (struct comm_s *cs);

int comm_set_outputs (struct comm_s *cs);

void _perror (const char *s)
{
	printf("ERROR: %s\n", s);
	return;
}

void _xerror (const char *s, const int32_t lret)
{
	_perror(s);
	char tmpbuf[BUFFER_SIZE] = {0};
	xDriverGetErrorDescription(lret, tmpbuf, BUFFER_SIZE);
	printf("Cause: %s\n", tmpbuf);
}

int _cifx_init (const char *spiport, CIFXHANDLE *driver, CIFXHANDLE *channel)
{
	int lret;
	struct CIFX_LINUX_INIT init =
	{
		.init_options        = CIFX_DRIVER_INIT_AUTOSCAN,
		.iCardNumber         = 0,
		.fEnableCardLocking  = 0,
		.base_dir            = spiport,
		.poll_interval       = 0,
		.poll_StackSize      = 0,
		.trace_level         = 255,
		.user_card_cnt       = 0,
		.user_cards          = NULL,
	};

	lret = cifXDriverInit(&init);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not initialize the driver", lret);
		goto exit_error;
	}

	lret = xDriverOpen(driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open driver", lret);
		goto exit_error;
	}

	lret = xChannelOpen(*driver, CIFX_DEV, 0, channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open channel", lret);
		goto exit_error;
	}


	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}

int _cifx_end (const CIFXHANDLE *driver, const CIFXHANDLE *channel)
{
	int lret;

	lret = xChannelClose(&channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to close the channel", lret);
		goto exit_error;
	}
	lret = xDriverClose(&driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to close the driver", lret);
		goto exit_error;
	}

	cifXDriverDeinit();

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}

int comm_init (struct comm_s *cs)
{
	int lret;
	if (NULL == cs->driver)
		cs->driver = (CIFXHANDLE *) malloc(sizeof(CIFXHANDLE));
	if (NULL == cs->channel)
		cs->channel = (CIFXHANDLE *) malloc(sizeof(CIFXHANDLE));


	lret = _cifx_init(cs->spiport, &cs->driver, &cs->channel);
//	lret |= xChannelReset(cs->channel, CIFX_SYSTEMSTART, cs->timeout);
	lret |= xChannelHostState(cs->channel, CIFX_HOST_STATE_READY, &cs->ulState, cs->timeout);
	lret |= xChannelConfigLock(cs->channel, CIFX_CONFIGURATION_UNLOCK, &cs->ulState, cs->timeout);

	return lret;
}

int comm_end (struct comm_s *cs)
{

	int lret = xChannelBusState(cs->channel, CIFX_BUS_STATE_OFF, &cs->ulState, cs->timeout);
	lret |= xChannelConfigLock(cs->channel, CIFX_CONFIGURATION_UNLOCK, &cs->ulState, cs->timeout);
	lret |= xChannelHostState(cs->channel, CIFX_HOST_STATE_NOT_READY, &cs->ulState, cs->timeout);

	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to set host state", lret);
	}
	return _cifx_end(&cs->driver, &cs->channel);
}
#ifdef SPINNER
void comm_bus_wait (struct comm_s *cs)
{
	int lret;
	char str[] = "Waiting for bus communication...";
	char spinner[4] = {'-', '\\', '|', '/'};
	short spinner_i = 0;
	do {
		printf("%s %c\r", str, spinner[spinner_i]);
		fflush(stdout);
		spinner_i = spinner_i < 3 ? spinner_i + 1 : 0;
		lret = xChannelBusState(cs->channel, CIFX_BUS_STATE_ON, &cs->ulState, cs->timeout);
	} while (CIFX_DEV_NO_COM_FLAG == lret);
	printf("%s OK\n", str);
}
#else
void comm_bus_wait (struct comm_s *cs)
{
	int lret;
	char str[] = "Waiting for bus communication...";
	do {
		printf("%s\n", str);
		lret = xChannelBusState(cs->channel, CIFX_BUS_STATE_ON, &cs->ulState, cs->timeout);
	} while (CIFX_DEV_NO_COM_FLAG == lret);
	puts(" OK");
}
#endif

int comm_bus_active (struct comm_s *cs)
{
	if (CIFX_DEV_NO_COM_FLAG == xChannelBusState(cs->channel, CIFX_BUS_STATE_ON, &cs->ulState, cs->timeout))
		return 0;
	else
		return 1;
}

int comm_bus_config_lock (struct comm_s *cs)
{
	int lret = xChannelConfigLock(cs->channel, CIFX_CONFIGURATION_LOCK, &cs->ulState, cs->timeout);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to lock configuration", lret);
		return -1;
	}
	return 0;
}

int comm_update_inputs (struct comm_s *cs)
{
	int lret = xChannelIORead(cs->channel, 0, 0, sizeof(cs->recvData), cs->recvData, cs->timeout);
	if (CIFX_NO_ERROR == lret)
		return 0;
	else
		return -1;
}

int comm_update_outputs (struct comm_s *cs)
{
	int lret = xChannelIOWrite(cs->channel, 0, 0, sizeof(cs->sendData), cs->sendData, cs->timeout);
	if (CIFX_NO_ERROR == lret)
		return 0;
	else
		return -1;
}

uint8_t comm_get_input_byte (struct comm_s *cs, int offset)
{
	return cs->recvData[offset];
}

uint16_t comm_get_input_word (struct comm_s *cs, int offset)
{
	return ((cs->recvData[offset] & 0xFF) << 8) + (cs->recvData[offset+1] & 0xFF);
}

uint8_t comm_get_input_bit (struct comm_s *cs, int byte_offset, int bit_offset)
{
	return comm_get_input_byte(cs, byte_offset) & (1 << bit_offset) != 0;
}
