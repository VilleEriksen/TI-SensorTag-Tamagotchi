# Description

Commlib source files with reception interval test.

In `address.h` it is possible to define an interval of addresses which will be received by this
SensorTag.

Diff of `comm_lib.c`:
```
--- comm_lib_old.c	2016-11-02 22:02:24.000000000 +0200
+++ comm_lib.c	2020-08-18 09:39:43.000000000 +0300
@@ -163,6 +163,13 @@ int8_t Receive6LoWPAN(uint16_t *senderAd
 	return i16_MACPDU_length;
 }
 
+bool isAddrInReceptionInterval(volatile uint8_t *entry) {
+	uint16_t senderAddr = ((CWC_CC2650_IEEE154_simple_packet_struct_t *)
+            (entry + CC2650_RX_ENTRY_HEADER_OVERHEAD_BYTES + CC2650_RX_ENTRY_ELEMENTLENGTH_BYTES + CC2650_RX_ENTRY_PHYHEADER_BYTES))
+            ->str_Header.SrcAddr;
+    return IEEE80154_RECEIVE_START <= senderAddr && senderAddr <= IEEE80154_RECEIVE_END;
+}
+
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //FunctionName:		Radio_IRQ
 ///Description:		Radio IRQ callback function
@@ -192,8 +199,12 @@ void Radio_IRQ(CWC_CC2650_154_Events_t E
 					nxt_entry = entry->pNextEntry;
 					CC2650_RXEntry_Release(tmp);
 				}
-				rx_read_entry=entry;
-				u8_RXd_Flag=1;
+				if (isAddrInReceptionInterval(entry)) {
+					rx_read_entry=entry;
+					u8_RXd_Flag=1;
+				} else {
+				    CC2650_RXEntry_Release(entry);
+				}
 			}
 			break;
 		case CWC_CC2650_154_EVENT_RXD_NOK:
@@ -207,8 +218,12 @@ void Radio_IRQ(CWC_CC2650_154_Events_t E
 					nxt_entry = entry->pNextEntry;
 					CC2650_RXEntry_Release(tmp);
 				}
-				rx_read_entry=entry;
-				u8_RXd_Flag=1;
+				if (isAddrInReceptionInterval(entry)) {
+					rx_read_entry=entry;
+					u8_RXd_Flag=1;
+				} else {
+				    CC2650_RXEntry_Release(entry);
+				}
 			}
 			break;
 		default:
```
