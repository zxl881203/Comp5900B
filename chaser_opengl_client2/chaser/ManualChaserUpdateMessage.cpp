#include "ManualChaserUpdateMessage.h"
#include <ctime>

#define TIME_SCALE 1000//the value is arbitrary, I need to modify it later

#define SHORT_TIME 3
#define MEDIUM_TIME 6
#define LONG_TIME 9


ManualChaserUpdateMessage::ManualChaserUpdateMessage(ShadowChaser sC)//this ocnstructor is needed for sending the message
{
	this->sC = sC;
}

ManualChaserUpdateMessage::ManualChaserUpdateMessage(gameApp* gA)//this constructor is needed for parsing the message
{
	this->gA = gA;
}



ManualChaserUpdateMessage::~ManualChaserUpdateMessage()
{
}

void ManualChaserUpdateMessage::sendMessage(SOCKET sockfd){
	int sendTime = std::time(0) * 1000;
	char* sendInformation = PackData(sockfd, sC.mPosition, sC.lookAtVector, sendTime, sC.getSpeed());

	int len;
	int rc;
	int i = 0, sLength;



	char buf[MSG_LENGTH];		// 256 is a magic number
	int continue2Recv = 1;

	sLength = strlen(sendInformation);
	while (i <= sLength) {
		// send the string until it has completed
		// note that the message size is artificially limited in length 
		len = (MSG_LENGTH - 1)<(sLength - i) ? (MSG_LENGTH - 1) : (sLength - i);	// send four bytes at a time

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
		memcpy(&buf[strlen(buf)], &sendInformation[i], currentPosition);	// copy part of the message

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


}

enum TimeDifference{
	SHORT, MEDIUM, LONG, INFI
};

void ManualChaserUpdateMessage::parseMessage(){
	int len;
	int rc;
	int i = 0, sLength;



	char buf[MSG_LENGTH];		// 256 is a magic number
	int continue2Recv = 1;

	// we know that the server echos the data back
	while (continue2Recv) {
		// wait for the server to send the messages back.  In this case it is synchronous.
		// the next message will not be sent until the original message was "echoed" back
		memset(buf, 0, MSG_LENGTH);
		rc = recv(this->gA->nc->sockfd, buf, MSG_LENGTH - 1, 0);    // wait until something was sent

		if (rc == 0) {
			// connecion is closed 
			rc = CONNECTION_CLOSED;
			break;
		}
		else if (rc == SOCKET_ERROR) {
			// error
			// could be that nothing is sent back. In this case you may want to leave the while loop.
			rc = WSAGetLastError();
			switch (rc) {
			case WSAEWOULDBLOCK:
				// connection was not established yet.  
				// do not call connect again on the same socket.

				continue2Recv = 0;
				rc = 0;
				break;
			case WSAECONNABORTED:
				rc = CONNECTION_CLOSED;
				break;

			default:
				rc = -1;
				break;
			}
		}
		else 	{
			cout << "client receeived -->" << buf << endl;
			//here we can modify the operation of the received message
		}
	}
	
	
	
	
	//update the corresponding manualChaser's position, lookAtVector, speed according to the "message delay interval"
	//read out the information
	Vector3f realPosition;//read from socket
	Vector3f realLookAtVector;//read from socket
	float realSpeed;
	double sentTime;//read from socket
	double currentTime = std::time(0) * 1000;
	enum TimeDifference timeDifference = (TimeDifference)((currentTime - sentTime) / TIME_SCALE);
	int timeOfCatchUp = -1;//define the time of catch up, the initialization is in the following switch: the larger time difference is, the longer catch up will be, 
	//however if it is over a certain range, we set the location and lookAtVector directly to the current one
	switch (timeDifference){
	case SHORT:
		timeOfCatchUp = SHORT_TIME;
		break;
	case MEDIUM:
		timeOfCatchUp = MEDIUM_TIME;

		break;
	case LONG:
		timeOfCatchUp = LONG_TIME;

		break;

	}

	if (timeOfCatchUp != -1){//if time of catch up is not too long to be manipulated
		//update the oldManualChaser information to the new information

		//I only need to know the manualChaser's current position which is the origin,
		//the future point which is calculated by using the transmitted mPosition
		//then I need to update the current this->gA->manualChaser's lookAtVector and speed


		ManualChaser *currentManualChaser= this->gA->manualChaser;

		//TIME_OF_CATCH_UP is set according to time of receiving message difference
		Vector3f futurePosition = realPosition + timeOfCatchUp*realLookAtVector;//I need to let my shadowChaser move to that position within 0.5 second
		currentManualChaser->lookAtVector = futurePosition - currentManualChaser->mPosition;
		currentManualChaser->lookAtVector.normalize();
		float dPF = (futurePosition - realPosition).length();//the distance between the chaser and the future position
		float dSF = (futurePosition - currentManualChaser->mPosition).length();//the distance between the shadowChaser and the future position
		float manualChaserSpeed = realSpeed;
		float increasedSpeed = manualChaserSpeed*dSF / dPF;
		currentManualChaser->setSpeed(increasedSpeed);
		currentManualChaser->moveForward(increasedSpeed);

		//recover the lookAtVector and speed
		currentManualChaser->lookAtVector = realLookAtVector;
		currentManualChaser->setSpeed(realSpeed);



	}
	else{//if tje update information is much too long, we need to brutely set the mLocaiton and lookAtVector of the gameApp's chaser
		ManualChaser *currentManualChaser = this->gA->manualChaser;
		currentManualChaser->mPosition = realPosition;
		currentManualChaser->lookAtVector = realLookAtVector;
		currentManualChaser->setSpeed(realSpeed);
	}


}