import Models.CyBotMessage as Message

from Services import CommunicationService


class CyBotMessageService:
    @staticmethod
    def translate(message_input: dict) -> Message.CyBotMessage:
        cls = Message.message_mapping[message_input.get("mes_type")]
        return cls(**message_input)

    def send_msg(self, msg: Message.CyBotMessage,
                 comm_srv: CommunicationService):
        comm_srv.send_dict(msg.json())
