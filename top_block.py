#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Fri Sep 24 17:30:31 2021
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from gnuradio import blocks
from gnuradio import channels
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import sip
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.scale = scale = 1
        self.samp_rate = samp_rate = 256000
        self.noiverVoltage = noiverVoltage = 4000
        self.freqOffset = freqOffset = 0

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	samp_rate, #bw
        	"", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)

        self.qtgui_sink_x_0.enable_rf_freq(False)



        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=noiverVoltage,
        	frequency_offset=freqOffset,
        	epsilon=1.0,
        	taps=(1.0 + 1.0j*0, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((scale, ))
        self.blocks_int_to_float_0_0 = blocks.int_to_float(1, 1)
        self.blocks_int_to_float_0 = blocks.int_to_float(1, 1)
        self.blocks_float_to_int_0_0 = blocks.float_to_int(1, 1)
        self.blocks_float_to_int_0 = blocks.float_to_int(1, 1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_file_source_0_0 = blocks.file_source(gr.sizeof_int*1, '/home/user/Public/gnuRadioOFDM_data_q.txt', False)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_int*1, '/home/user/Public/gnuRadioOFDM_data_i.txt', False)
        self.blocks_file_sink_0_1 = blocks.file_sink(gr.sizeof_int*1, '/home/user/Public/gnuRadioOFDM_data_res_i.txt', False)
        self.blocks_file_sink_0_1.set_unbuffered(False)
        self.blocks_file_sink_0_0_0 = blocks.file_sink(gr.sizeof_int*1, '/home/user/Public/gnuRadioOFDM_data_res_q.txt', False)
        self.blocks_file_sink_0_0_0.set_unbuffered(False)
        self.blocks_complex_to_float_0_0 = blocks.complex_to_float(1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_float_0_0, 0), (self.blocks_float_to_int_0, 0))
        self.connect((self.blocks_complex_to_float_0_0, 1), (self.blocks_float_to_int_0_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_int_to_float_0, 0))
        self.connect((self.blocks_file_source_0_0, 0), (self.blocks_int_to_float_0_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.channels_channel_model_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.blocks_float_to_int_0, 0), (self.blocks_file_sink_0_1, 0))
        self.connect((self.blocks_float_to_int_0_0, 0), (self.blocks_file_sink_0_0_0, 0))
        self.connect((self.blocks_int_to_float_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_int_to_float_0_0, 0), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_complex_to_float_0_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.blocks_multiply_const_vxx_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_scale(self):
        return self.scale

    def set_scale(self, scale):
        self.scale = scale
        self.blocks_multiply_const_vxx_0.set_k((self.scale, ))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_noiverVoltage(self):
        return self.noiverVoltage

    def set_noiverVoltage(self, noiverVoltage):
        self.noiverVoltage = noiverVoltage
        self.channels_channel_model_0.set_noise_voltage(self.noiverVoltage)

    def get_freqOffset(self):
        return self.freqOffset

    def set_freqOffset(self, freqOffset):
        self.freqOffset = freqOffset
        self.channels_channel_model_0.set_frequency_offset(self.freqOffset)


def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
