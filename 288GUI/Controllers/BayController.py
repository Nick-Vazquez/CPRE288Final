import logging
import queue

from Components.BayOccupancyWidget import BayOccupancyWidget
from Services.CyBotMessageService import Subscriber
from Components.ScanPlotterView import PlotterView
from Models.ScanResults import ScanResult
from Models.CyBotMessage import ScanResultsMessage, ScanUpdateType, \
    BayResultsMessage

logger = logging.getLogger(__name__)


class BayController(Subscriber):
    def __init__(self, parent):
        super(BayController, self).__init__()
        self.parent = parent
        self.accepts = BayResultsMessage
        self.view = BayOccupancyWidget(parent)
        self.poll_input_queue()

    def poll_input_queue(self):
        try:
            msg = self.input_queue.get_nowait()
            msg: BayResultsMessage
            logger.debug(f"Updating bay: {msg.bay_num} - {msg.occupancy}")
            self.view.occupancies[msg.bay_num-1].set(msg.occupancy)
        except queue.Empty:
            pass
        self.parent.after(200, self.poll_input_queue)
