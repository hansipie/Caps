
void MyFrame::output_message(wxString str)
{
	int len, n;
	char *temp;
	char *msg;
wxString aff;

	if (is_connected_console == 0)
		return;

//	m_panel->m_text->AppendText(msg);


	if (0)
	{
		temp = crypto( str, "mot de passe nul", 'c');
		temp = crypto(temp, "toto est tres bo", 'c');

		temp = crypto(temp, "toto est tres bo", 'd');
		temp = crypto(temp, "mot de passe nul", 'd');

	}
	else
		temp = tocharetoile(str); 

	len = strlen(temp);
	msg = new char[len + 3];
	for (n = 0; n < len; n ++)
		msg[n] = temp[n];
	msg[n] = 13;
	msg[n + 1] = 10;
	msg[n + 2] = 0;

	/*aff = "j'envoie au console : -";
	aff += msg;
	aff += "--";*/
		//wxMessageBox(temp);

	m_busy = TRUE;
	m_sock->Write(msg, len);
	m_busy = FALSE;
}

