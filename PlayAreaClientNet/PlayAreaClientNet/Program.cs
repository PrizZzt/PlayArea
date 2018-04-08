using System;

namespace PlayAreaClientNet
{
	class Program
	{
		static void Main(string[] args)
		{
			var r = new Random();
			GameClient client = new GameClient();
			client.OnTurn += () =>
			{
				switch (r.Next(5))
				{
					case 0:
						if (client.PlayerY > 0 && IsBlock(client.Map[client.PlayerX, client.PlayerY - 1]) == false)
							client.Up();
						break;

					case 1:
						if (client.PlayerX + 1 < client.SizeX && IsBlock(client.Map[client.PlayerX + 1, client.PlayerY]) == false)
							client.Right();
						break;

					case 2:
						if (client.PlayerY + 1 < client.SizeY && IsBlock(client.Map[client.PlayerX, client.PlayerY + 1]) == false)
							client.Down();
						break;

					case 3:
						if (client.PlayerX > 0 && IsBlock(client.Map[client.PlayerX - 1, client.PlayerY]) == false)
							client.Left();
						break;

					case 4:
						client.Act();
						break;
				}
			};
			client.Run("127.0.0.1", 1234, "NetClient", "Password");
		}

		private static bool IsBlock(EBombermanObjects block) =>
			block != EBombermanObjects.NONE &&
			block != EBombermanObjects.DESTROYED_WALL &&
			block != EBombermanObjects.DEAD_PLAYER &&
			block != EBombermanObjects.DEAD_MEATCHOPPER &&
			block != EBombermanObjects.BOOM;
	}
}
