package com.atmel.example.ap_provision;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;

import com.atmel.example.ap_provision.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final String TAG = "PROVISION_AP";

	// Dialog Message
	private static final int DIALOG_SSID = 1;
	private static final int DIALOG_SOCKET_TIMEOUT_ERROR = 2;
	private static final int DIALOG_SOCKET_COMPLETE = 3;
	private static final int DIALOG_SSID_NULL = 4;
	private static final int DIALOG_SECURITY_NULL = 5;
	private static final int DIALOG_PASSWORD_NULL = 6;

	// Handle message
	private static final int SOCKET_CONNECT = 1000;
	private static final int SERVER_FROM_MESSAGE = 1001;
	private static final int SERVER_FROM_MESSAGE_CONFIRM = 1002;
	private static final int SERVER_FROM_MESSAGE_APPLIED = 1003;
	private static final int EDIT_SSID_NULL = 1010;
	private static final int EDIT_SECURITY_NULL = 1011;
	private static final int EDIT_PASSWORD_NULL = 1012;

	// Error Handle
	private static final int SOCKET_TIMEOUT_ERROR = 1100;
	private static final int SOCKET_SEND_FAIL = 1101;

	// Socket Server setting
	private static String server_ip = "192.168.1.1";
	// private static int server_port = 20000;
	private static int server_port = 80;
	private Socket socket = null;

	// Socket value
	PrintWriter out;
	BufferedReader in;

	// wifi connection
	WifiConfiguration wfc;

	// Button
	Button ssidchange_buttons, connectButton;
	// TextView
	TextView tv_currentstate, tv_ssid, tv_security, tv_password;

	volatile boolean bTcpCancel = false;

	// Data format
	// 'confirm' 'SSID' 'Security' 'Password'
	private String data_confirm;
	private String data_ssid = null;
	private String data_security = null;
	private String data_passwd = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		connectButton = (Button) findViewById(R.id.connect_buttons);
		connectButton.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Thread cThread = new Thread(new SocClient());
				cThread.start();
			}
		});

		ssidchange_buttons = (Button) findViewById(R.id.ssidchange_buttons);
		ssidchange_buttons.setOnClickListener(new OnClickListener() {
			@SuppressWarnings("deprecation")
			public void onClick(View v) {
				showDialog(DIALOG_SSID);
			}
		});
		ssidchange_buttons.setEnabled(false);

		tv_currentstate = (TextView) findViewById(R.id.tv_currentstate);
		tv_ssid = (TextView) findViewById(R.id.tv_ssid);
		tv_security = (TextView) findViewById(R.id.tv_security);
		tv_password = (TextView) findViewById(R.id.tv_password);

		Button currentAP_Name = (Button) findViewById(R.id.get_apname);
		currentAP_Name.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				getcurrentAP_Name(getApplicationContext());
			}
		});

		Button button_finish = (Button) findViewById(R.id.bt_finish);
		button_finish.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				finish();
			}
		});
	}

	final Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			if (msg.what == SOCKET_CONNECT) {
				connectButton.setText("MSP Connected");
				ssidchange_buttons.setEnabled(true);
				Log.d(TAG, "Handle : SOCKET_CONNECT");
			} else if (msg.what == SERVER_FROM_MESSAGE) {
				tv_currentstate.setText("Below recent settings");
				if (data_ssid != null)
					tv_ssid.setText("SSID : " + data_ssid);
				if (data_security != null)
					tv_security.setText("Security : " + getSecurity_Name(Integer.parseInt(data_security)));
				if (data_passwd != null)
					tv_password.setText("Password : " + data_passwd);
				Log.d(TAG, "Handle : SERVER_FROM_MESSAGE");
			} else if (msg.what == SOCKET_TIMEOUT_ERROR
					|| msg.what == SOCKET_SEND_FAIL) {
				showDialog(DIALOG_SOCKET_TIMEOUT_ERROR);
				Log.d(TAG, "Handle : SOCKET_TIMEOUT_ERROR");
			} else if (msg.what == SERVER_FROM_MESSAGE_APPLIED) {
				showDialog(DIALOG_SOCKET_COMPLETE);
				Log.d(TAG, "Handle : DIALOG_SOCKET_COMPLETE");
			} else if (msg.what == EDIT_SSID_NULL) {
				Log.d(TAG, "Handle : DIALOG_SSID_NULL");
				showDialog(DIALOG_SSID_NULL);
			} else if (msg.what == EDIT_SECURITY_NULL) {
				Log.d(TAG, "Handle : DIALOG_SECURITY_NULL");
				showDialog(DIALOG_SECURITY_NULL);
			} else if (msg.what == EDIT_PASSWORD_NULL) {
				Log.d(TAG, "Handle : DIALOG_PASSWORD_NULL");
				showDialog(DIALOG_PASSWORD_NULL);
			}
		}
	};

	public class SocClient implements Runnable {
		char[] BUF = null;

		String str;
		String message = "connect";
		String temp[];
		int count = 0;

		public void run() {
			try {
				InetAddress serverAddr = InetAddress.getByName(server_ip);
				InetSocketAddress socketAddress = new InetSocketAddress(
						serverAddr, server_port);

				socket = new Socket();
				if (socket != null) {
					socket.connect(socketAddress, 5000);
					handler.sendEmptyMessage(SOCKET_CONNECT);
				}

				try {
					// Message 'admin' is connection key
					Log.d(TAG, "C : Send message  : '" + message + "'");
					// Server send data
					out = new PrintWriter(new BufferedWriter(
							new OutputStreamWriter(socket.getOutputStream())),
							true);
					out.println(message);
					Log.d(TAG, "C : Sent. Done");

					// Temp data Max 200 bytes
					BUF = new char[200];
					// client recieve data
					in = new BufferedReader(new InputStreamReader(
							socket.getInputStream()));

					while (socket.isConnected()) {
						int len = 0;
						try {
							len = in.read(BUF);
						} catch (IOException e) {
							e.printStackTrace();
						}

						if (len > 0) {

							final String Msg_From_Client = new String(BUF);

							Log.d(TAG, "Msg_From_Client [" + count + "] : "
									+ Msg_From_Client.substring(0, len));

							// Server from information
							switch (count) {
							case 0:
								data_confirm = Msg_From_Client.substring(0, len);
								break;
							case 1:
								data_ssid = Msg_From_Client.substring(0, len);
								break;
							case 2:
								data_security = Msg_From_Client.substring(0,
										len);
								break;
							case 3:
								data_passwd = Msg_From_Client.substring(0, len);
								handler.sendEmptyMessage(SERVER_FROM_MESSAGE);
								break;
							case 4:
								if (Msg_From_Client.substring(0, len).equals(
										"applied")) {
									handler.sendEmptyMessage(SERVER_FROM_MESSAGE_APPLIED);
								}
							case 5:
								// Not used
								break;

							default:
								break;
							}
							count++;
						}

					}
				} catch (Exception e) {
					handler.sendEmptyMessage(SOCKET_SEND_FAIL);
					Log.e(TAG, "C : Error", e);
				} finally {
					Log.d(TAG, "C : socket.close ");
					socket.close();
				}
			} catch (IOException e) {
				handler.sendEmptyMessage(SOCKET_TIMEOUT_ERROR);
				Log.e(TAG, "C : Connection Time out - 5 sec", e);
			}
		}
	}

	private String getSecurity_Name(int index) {

		switch (index) {
		case 1:
			return "Open";
		case 2:
			return "WPA_WPA2";
		case 3:
			return "WEP";
		default:
			return "Error";
		}
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		switch (id) {
		case DIALOG_SSID:
			LayoutInflater factory = LayoutInflater.from(this);
			final View textEntryView = factory.inflate(
					R.layout.alert_dialog_text_entry, null);
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setTitle("SSID Change")
					.setView(textEntryView)
					.setPositiveButton("Apply",
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									/* User clicked OK so do some stuff */
									EditText ed_ssid = (EditText) textEntryView
											.findViewById(R.id.username_edit);
									EditText ed_sec = (EditText) textEntryView
											.findViewById(R.id.ed_security);
									EditText ed_pass = (EditText) textEntryView
											.findViewById(R.id.password_edit);

									data_ssid = ed_ssid.getText().toString();
									data_security = ed_sec.getText().toString();
									data_passwd = ed_pass.getText().toString();

									if (ed_ssid.getText().length() < 1) {
										handler.sendEmptyMessage(EDIT_SSID_NULL);
									} else if (ed_sec.getText().length() < 1
											|| Integer.parseInt(data_security) > 3
											|| Integer.parseInt(data_security) < 1) {
										handler.sendEmptyMessage(EDIT_SECURITY_NULL);
									} else if (ed_pass.getText().length() < 1
											&& Integer.parseInt(data_security) > 1) {
										handler.sendEmptyMessage(EDIT_PASSWORD_NULL);
									}

									if (data_ssid != null
											&& data_passwd != null
											&& data_security != null) {
										Log.d(TAG, "add Network ssid "
												+ data_ssid);
										Log.d(TAG, "add Network security "
												+ data_security);
										Log.d(TAG, "add Network key "
												+ data_passwd);
									} else {
										Log.d(TAG, "add Network string null ");
									}

									// Send to AP info
									if (out != null)
										out.println("apply," + data_ssid + ","
												+ data_security + ","
												+ data_passwd + ",");
									Log.d(TAG, "Send String : " + "apply,"
											+ data_ssid + "," + data_security
											+ "," + data_passwd + ",");
								}
							})
					.setNegativeButton(android.R.string.no,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									dialog.cancel();
								}
							}).create();

		case DIALOG_SOCKET_TIMEOUT_ERROR:
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setCancelable(false)
					.setTitle("SOCKET TIMEOUT ERROR")
					.setMessage("Please Retry")
					.setPositiveButton(android.R.string.ok,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									dialog.cancel();
									finish();
								}
							}).create();

		case DIALOG_SOCKET_COMPLETE:
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setCancelable(false)
					.setTitle("AP change Complete")
					.setMessage("Good Job ^^")
					.setPositiveButton(android.R.string.ok,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									if (socket != null && socket.isConnected()) {
										try {
											Log.d(TAG, "socket close !!!");
											socket.close();
										} catch (IOException e) {
											e.printStackTrace();
										}
									}
									dialog.cancel();
									finish();
								}
							}).create();

		case DIALOG_SSID_NULL:
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setCancelable(false)
					.setTitle("SSID Empty field")
					.setMessage("Check please, Max 20 character")
					.setPositiveButton(android.R.string.ok,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									dialog.cancel();
								}
							}).create();

		case DIALOG_SECURITY_NULL:
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setCancelable(false)
					.setTitle("Security Empty field")
					.setMessage("Check please, 1:OPEN, 2:WPA_WPA2, 3:WEP")
					.setPositiveButton(android.R.string.ok,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									dialog.cancel();
								}
							}).create();

		case DIALOG_PASSWORD_NULL:
			return new AlertDialog.Builder(MainActivity.this)
					.setIconAttribute(android.R.attr.alertDialogIcon)
					.setCancelable(false)
					.setTitle("Password Empty field")
					.setMessage("Your security type not Open, Please check")
					.setPositiveButton(android.R.string.ok,
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog,
										int whichButton) {
									dialog.cancel();
								}
							}).create();

		default:
			return null;
		}
	}

	private void getcurrentAP_Name(Context context) {
		WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		String ssid = wm.getConnectionInfo().getSSID();
		Toast.makeText(context, "Current AP : " + ssid, Toast.LENGTH_LONG)
				.show();
	}

	@Override
	protected void onStop() {
		bTcpCancel = true;
		System.exit(RESULT_OK);
		super.onStop();
	}

	@Override
	protected void onDestroy() {
		Log.d(TAG, "onDestroy !!!");
		if (socket.isConnected()) {
			try {
				Log.d(TAG, "socket close !!!");
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		super.onDestroy();
	}
}
