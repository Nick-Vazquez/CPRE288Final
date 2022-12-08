"""Acts as interface between the GUI application and Bay occupancy display.

__created__ = 2022/12/06
__author__ = Nick Vazquez (nmv)
"""
import logging
import queue

from Components.BayOccupancyWidget import BayOccupancyWidget
from Models.CyBotMessage import BayResultsMessage
from Services.CyBotMessageService import Subscriber

logger = logging.getLogger(__name__)


class BayController(Subscriber):
    """Controller object that holds model and view."""
    def __init__(self, parent):
        super(BayController, self).__init__()
        self.parent = parent
        self.accepts = BayResultsMessage
        self.view = BayOccupancyWidget(parent)
        self.poll_input_queue()

    def poll_input_queue(self):
        """Check the input queue for any BayResultsMessages, and update the
        corresponding model and view."""
        try:
            msg = self.input_queue.get_nowait()
            msg: BayResultsMessage
            logger.debug(f"Updating bay: {msg.bay_num} - {msg.occupancy}")
            self.view.occupancies[msg.bay_num-1].set(msg.occupancy)
        except queue.Empty:
            pass
        self.parent.after(200, self.poll_input_queue)
