#include <string>
#include <iostream>

#include "libepp_nicbr_cli.H"

#define   PORT_FILE    "../unit_test/SessionTestServer.port"


/***** To run this program: ***********
       make
       make test
*************************************/

using namespace std;
LIBEPP_NICBR_NS_USE

auto_ptr<Session> _session;
auto_ptr<DomParser> _command_parser;   

void connect() 
{
  printf("Connecting to the server....");
  try { 
    _session->connect("../unit_test/client.pem", 
		      "../unit_test/root.pem", 
		      "client passphrase");

    printf("OK\n");
    printf("Receiving greeting from the server....");

    Greeting *greeting = _session->get_greeting();  

    printf("OK\n");
  } catch (const IoException &e) {
    printf("\nIO Exception: \ncode [%d]\n message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const GeneralException &e) {
    printf("\nGeneral Exception: \ncode [%d]\n message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }
  
}

void login() 
{
  printf("Sending login command....");
  
  try {
    Login my_login;
    LoginCmd *login_cmd = my_login.get_command();

    login_cmd->set_clID("ClientX");
    login_cmd->set_pw("foo-BAR2");
    login_cmd->set_new_pw("bar-FOO2");

    string cmd_clTRID = "ABC-12345";
    _session->process_action(&my_login, cmd_clTRID);

    Response *login_resp = my_login.get_response();

    printf("OK\n");
  } catch (const EppException &e) {
    printf("\nEpp Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());  
  } catch (const IoException &e) {
    printf("\nTransport Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const GeneralException &e) {
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } 
  
}

void create_contact() 
{
  printf("Sending a contact create command....");

  try {
    ContactCreate my_contact_create;
    ContactCreateCmd *contact_create_cmd = my_contact_create.get_command();

    CommonData common_data;

    common_data.set_id("sh8013");
    
    PostalInfo postal_info; 
    postal_info.set_type("int");
    postal_info.set_name("John Doe");
    postal_info.set_org("Example Inc.");
    postal_info.set_str1("123 Example Dr.");
    postal_info.set_str2("Suite 100");
    postal_info.set_str3("");
    postal_info.set_city("Dulles");
    postal_info.set_sp("VA");
    postal_info.set_pc("20166-6503");
    postal_info.set_cc("US");

    common_data.insert_postal_info(postal_info);

    CommonData::Phone phone;
    phone.ext = "1234";
    phone.number = "+1.7035555555";
    common_data.set_voice(phone);

    phone.ext = "";
    phone.number = "+1.7035555556";
    common_data.set_fax(phone);

    string email = "jdoe@example.com";
    common_data.set_email(email);
    
    AuthInfo authInfo;
    authInfo.set_roid("");
    authInfo.set_pw("2fooBAR");
    contact_create_cmd->set_authInfo(authInfo);

    CommonData::Disclose disclose;
    disclose.name_int = true;
    common_data.set_disclose(disclose);

    contact_create_cmd->set_common_data(common_data);

    string cmd_clTRID = "ABC-12345";
    _session->process_action(&my_contact_create, cmd_clTRID);

    ContactCreateRsp *contact_create_resp = my_contact_create.get_response();

    printf("OK\n");
  } catch (const EppException &e) {
    printf("\nEpp Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());  
  } catch (const GeneralException &e) {
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }
  
}

void hello()
{
  printf("Sending a hello command....");
 
  try {
    _session->send_hello();
    
    Greeting *greeting = _session->get_greeting();  
    
    printf("OK\n");
    
  } catch (const EppException &e) {
    printf("\nEpp Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());  
  } catch (const GeneralException &e) {
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }
}

void logout() 
{
  printf("Sending logout command....");
  try {
    Logout my_logout;
    string cmd_clTRID = "ABC-12345";

    _session->process_action(&my_logout, cmd_clTRID);
    
    Response *logout_rsp = my_logout.get_response();

    printf("OK\n");
  } catch (const EppException &e) {
    printf("\nEpp Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());  
  } catch (const XmlException &e) {
    printf("\nXML Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const IoException &e) {
    printf("\nTransport Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const GeneralException &e) {
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } 
  
}

int main(int argc, char **argv) {

  int port;
  FILE *fd = fopen(PORT_FILE, "r");
  if (fd == NULL) {
    printf("File %s not found\n", PORT_FILE);
    return 0;
  }
  fscanf(fd, "%d", &port);
  fclose(fd);
  const string server = "localhost";
  try {

    _session = auto_ptr<Session>(new Session(server, port));

    _session->enable_xml_validation();

    connect();

    login();

    create_contact();

    hello();

    logout();
 } catch (const GeneralException &e) {
   printf("\nGeneral Exception: code [%d] message [%s]\n", 
	  e.get_code(), e.get_msg().c_str());
 }
 return 0;
}
