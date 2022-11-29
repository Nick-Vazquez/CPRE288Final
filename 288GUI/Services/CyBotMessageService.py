import Models.CyBotMessage as message


class CyBotMessageService:
    @staticmethod
    def translate(message_input: dict) -> message.CyBotMessage:
        cls = message.message_mapping[message_input.get("mes_type")]
        return cls(**message_input)
