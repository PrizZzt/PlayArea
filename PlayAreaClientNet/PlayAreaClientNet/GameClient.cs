using System;
using System.Net.Sockets;

namespace PlayAreaClientNet
{
	class GameClient
	{
		private TcpClient tcpClient = new TcpClient();
		private NetworkStream netStream;
		
		public EBombermanObjects[,] Map { get; set; }
		public byte SizeX { get; set; }
		public byte SizeY { get; set; }
		public event Action OnTurn;
		public byte PlayerID { get; set; }
		public byte PlayerX { get; set; }
		public byte PlayerY { get; set; }

		public GameClient()
		{
			tcpClient = new TcpClient();

			Map = null;
			SizeX = 0;
			SizeY = 0;
		}

		public void Run(string host, int port, string login, string password)
		{
			tcpClient.Connect(host, port);
			netStream = tcpClient.GetStream();

			if (login.Length > 20)
				throw new ArgumentOutOfRangeException(nameof(login));
			if (password.Length > 20)
				throw new ArgumentOutOfRangeException(nameof(password));

			int credentialsSize = 41;
			byte[] credentials = new byte[credentialsSize];
			credentials[0] = 6;//CREDENTIALS
			for (int i = 0; i < login.Length; i++)
			{
				credentials[1 + i] = Convert.ToByte(login[i]);
			}
			for (int i = 0; i < password.Length; i++)
			{
				credentials[21 + i] = Convert.ToByte(password[i]);
			}
			netStream.Write(credentials, 0, credentialsSize);

			byte[] credentialsAnswer = new byte[2];
			netStream.Read(credentialsAnswer, 0, 2);
			if (credentialsAnswer[0] == 2)
			{
				PlayerID = credentialsAnswer[1];
			}
			else
				throw new Exception("Login was failed");

			byte[] mapSize = new byte[2];
			byte[] objectInfo = new byte[2];
			int received;
			while (true)
			{
				while ((received = netStream.ReadByte()) == -1) ;
				byte messageType = (byte)received;
				if (messageType == 1)
				{
					netStream.Read(mapSize, 0, 2);
					if (SizeX != mapSize[0] || SizeY != mapSize[1])
					{
						SizeX = mapSize[0];
						SizeY = mapSize[1];
						Map = new EBombermanObjects[SizeX, SizeY];
					}
					for (byte j = 0; j < SizeY; j++)
					{
						for (byte i = 0; i < SizeX; i++)
						{
							netStream.Read(objectInfo, 0, 2);
							Map[i, j] = (EBombermanObjects)objectInfo[0];
							if (IsPlayer(Map[i, j]) && objectInfo[1] == PlayerID)
							{
								PlayerX = i;
								PlayerY = j;
							}
						}
					}
				}
				else
					netStream.Flush();

				OnTurn();
			}

			//tcpClient.Close();
		}

		public void Up()
		{
			netStream.WriteByte(1);
			byte[] actionAnswer = new byte[2];
			netStream.Read(actionAnswer, 0, 2);
		}

		public void Down()
		{
			netStream.WriteByte(2);
			byte[] actionAnswer = new byte[2];
			netStream.Read(actionAnswer, 0, 2);
		}

		public void Right()
		{
			netStream.WriteByte(3);
			byte[] actionAnswer = new byte[2];
			netStream.Read(actionAnswer, 0, 2);
		}

		public void Left()
		{
			netStream.WriteByte(4);
			byte[] actionAnswer = new byte[2];
			netStream.Read(actionAnswer, 0, 2);
		}

		public void Act()
		{
			netStream.WriteByte(5);
			byte[] actionAnswer = new byte[2];
			netStream.Read(actionAnswer, 0, 2);
		}

		private bool IsPlayer(EBombermanObjects obj) =>
			obj == EBombermanObjects.PLAYER ||
			obj == EBombermanObjects.PLAYER_WITH_BOMB_1 ||
			obj == EBombermanObjects.PLAYER_WITH_BOMB_2 ||
			obj == EBombermanObjects.PLAYER_WITH_BOMB_3 ||
			obj == EBombermanObjects.PLAYER_WITH_BOMB_4 ||
			obj == EBombermanObjects.PLAYER_WITH_BOMB_5 ||
			obj == EBombermanObjects.DEAD_PLAYER;
	}
}
