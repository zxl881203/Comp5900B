#define MSG_LENGTH 256


#include "UpdateMessage.h"


UpdateMessage::UpdateMessage()
{
}


UpdateMessage::~UpdateMessage()
{
}

char *UpdateMessage::PackData(SOCKET sockfd, Vector3f position, Vector3f lookAtVector, int sendTime, float speed){

	char *datapack;
	string str1, str2, str3, str4;
	str1 = vectortostring(position);
	str2 = vectortostring(lookAtVector);
	str3 = stringify(sendTime);
	str4 = stringify(speed);
	string final = str1 + ' ' + str2 + ' ' + str3 + ' ' + str4;
	datapack = new char[final.length() + 1];
	strcpy(datapack, final.c_str());

	char* msg = datapack;

	int len;
	int rc;
	int i = 0, sLength;



	char buf[MSG_LENGTH];		// 256 is a magic number
	int continue2Recv = 1;

	sLength = strlen(msg);
	while (i <= sLength) {
		// send the string until it has completed
		// note that the message size is artificially limited in length 
		len = (MSG_LENGTH - 1) < (sLength - i) ? (MSG_LENGTH - 1) : (sLength - i);	// send four bytes at a time

		// in this case the message is sent only once and the client asks for the connectin to be termineated.  
		// this does not have to be the case. The connection can be maintained if needed. 
		if (len == 0) {
			//end of string
			//shutdown(sockfd,1);
			break;
		}

		// copy part of the message to be sent
		memset(buf, 0, MSG_LENGTH);			// clear the memory
		strcpy(buf, "[CLIENT 1] ");

		//memcpy(&buf[strlen(buf)], &sMy[i],len-strlen(buf));	// copy part of the message
		int currentPosition;
		if (len + strlen(buf) > MSG_LENGTH - 1) currentPosition = len - strlen(buf);
		else currentPosition = len;
		memcpy(&buf[strlen(buf)], &msg[i], currentPosition);	// copy part of the message

		cout << "CLIENT 1 sending -> " << buf << endl;


		// send the message
		rc = strlen(buf);
		rc = send(sockfd, buf, strlen(buf), 0);   // note the number of bytes that were sent can be smaller then 
		if (rc == SOCKET_ERROR) {
			// error
			rc = -1;
			break;
		}

		i += len;
		if (rc != len) {
			// not all bytes were sent 
			// need loop
			// ignore for now
			rc = 5;
		}

		if (rc == 0) {
			//end of string
			//shutdown(sockfd,1);
			break;
		}
	}

	return msg;
}

temp UpdateMessage::UnpackData(char str[]){
	temp tp;
	double	time;
	float speed;
	Vector3f position;
	Vector3f look;
	string strin = str;
	//remove head;
	string newstr = strin.substr(9);
	string buf;
	stringstream ss(newstr);
	vector<string> arr;
	while (ss >> buf)
		arr.push_back(buf);

	position.x = atof(arr[0].c_str());
	position.y = atof(arr[1].c_str());
	position.z = atof(arr[2].c_str());
	look.x = atof(arr[3].c_str());
	look.y = atof(arr[4].c_str());
	look.z = atof(arr[5].c_str());
	time = atof(arr[6].c_str());
	speed = atof(arr[7].c_str());
	tp.myposition = position;
	tp.lookatVector = look;
	tp.sendtime = time;
	tp.speed = speed;
	return tp;
}


string UpdateMessage::vectortostring(Vector3f position){
	string str1 = stringify(position.x);
	string str2 = stringify(position.y);
	string str3 = stringify(position.z);
	string str = str1 + ' ' + str2 + ' ' + str3;
	return str;
}



void UpdateMessage::messageSend(SOCKET sockfd){
		
}

void UpdateMessage::messageParse(SOCKET sockfd, char*){
	
}