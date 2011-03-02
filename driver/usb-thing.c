/* I changed it a little. Do whatever the license tag in the other thing says. -- ik */

/* Name: set-led.c
 * Project: hid-custom-rq example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id: set-led.c 692 2008-11-07 15:07:40Z cs $
 */

/*
This program must be linked with libusb on Unix and libusb-win32 on Windows.
See http://libusb.sourceforge.net/ or http://libusb-win32.sourceforge.net/
respectively.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>
#include "opendevice.h" /* common code */

#include "../firmware/requests.h"   /* custom request numbers */
#include "../firmware/usbconfig.h"  /* device's VID/PID and names */

static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s 1 ....... turn on transistor\n", name);
    fprintf(stderr, "  %s 0 ....... turn off transistor\n", name);
}

int main(int argc, char **argv)
{
  usb_dev_handle *handle = NULL;
  const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}; 
  const unsigned char rawPid[2] = {USB_CFG_DEVICE_ID};
  char vendor[] = {USB_CFG_VENDOR_NAME, 0};
  char product[] = {USB_CFG_DEVICE_NAME, 0};
  char buffer[4];
  int cnt, vid, pid;

  usb_init();
  if (argc != 2){
    usage(argv[0]);
    exit(1);
  }
  /* compute VID/PID from usbconfig.h */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* The following function is in opendevice.c: */
    if (usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0) {
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        exit(1);
    }

    int r = atoi(argv[1]);
    if (r != 0 && r != 1) {
        usage(argv[0]);
        exit(1);
    }
    cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_SET_RED, r, 0, buffer, 0, 5000);
    usb_close(handle);
    return 0;
}
