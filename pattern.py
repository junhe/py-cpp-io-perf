#!/usr/bin/python
import os, sys
from time import time
import random

READ, WRITE, DISCARD = ('opread', 'opwrite', 'opdiscard')
BYTE, KB, MB, GB, TB = [2**(10*i) for i in range(5)]

def assert_multiple(n, divider):
    "n is multiple of divider"
    assert n % divider == 0, "{} is not mutliple of {}".format(n, divider)

class Request(object):
    def __init__(self, op, offset, size):
        self.op = op
        self.offset = offset
        self.size = size

    def __str__(self):
        return "{} {} {}".format(self.op, self.offset, self.size)


class PatternBase(object):
    def __iter__(self):
        raise NotImplementedError()


class InitMixin(object):
    def __init__(self, op, zone_offset, zone_size, chunk_size, traffic_size):
        self.op = op
        self.zone_offset = zone_offset
        self.zone_size = zone_size
        self.chunk_size = chunk_size
        self.traffic_size = traffic_size

        assert_multiple(n=traffic_size, divider=chunk_size)
        assert_multiple(n=zone_size, divider=chunk_size)

class Random(PatternBase, InitMixin):
    def __iter__(self):
        n_req = self.traffic_size / self.chunk_size
        n_chunks = self.zone_size / self.chunk_size
        chunk_idx = list(range(n_chunks))

        for i in range(n_req):
            chunk_id = random.choice(chunk_idx)
            req_offset = chunk_id * self.chunk_size
            req_size = self.chunk_size
            req = Request(self.op, req_offset, req_size)
            yield req

        raise StopIteration


class Sequential(PatternBase, InitMixin):
    def __iter__(self):
        n_req = self.traffic_size / self.chunk_size
        n_chunks = self.zone_size / self.chunk_size

        for i in range(n_req):
            chunk_id = i % n_chunks
            req_offset = chunk_id * self.chunk_size
            req_size = self.chunk_size
            req = Request(self.op, req_offset, req_size)
            yield req

        raise StopIteration


def access_file(filepath, pattern_iter):
    fd = os.open(filepath, os.O_RDWR|os.O_CREAT )
    for req in pattern_iter:
        if req.op == WRITE:
            os.lseek(fd, req.offset, os.SEEK_SET)
            os.write(fd, 'x' * req.size)
            # os.fsync(fd)
        elif req.op == READ:
            os.lseek(fd, req.offset, os.SEEK_SET)
            os.read(fd, req.size)

    os.close( fd )


class Experiment(object):
    def __init__(self, exp_name, classname, **kwargs):
        self.paras = kwargs
        self.exp_name = exp_name
        self.classname = classname

    def run(self):
        start = time()

        patclass = eval(self.classname)
        pat_iter = patclass(**self.paras)
        access_file(self.exp_name + '.data', pat_iter)

        end = time()
        dur = end - start

        self.print_stats(dur)

    def print_stats(self, duration):
        print self.exp_name, duration


def main():
    exps = [
            {'exp_name':'RandomWrite001',
            'classname':'Random',
            'op':WRITE, 'zone_offset':0, 'zone_size':128*MB,
            'chunk_size':2*KB, 'traffic_size':128*MB},

            {'exp_name':'SequentialWrite001',
            'classname':'Sequential',
            'op':WRITE, 'zone_offset':0, 'zone_size':128*MB,
            'chunk_size':2*KB, 'traffic_size':128*MB}]

    for setting in exps:
        expobj = Experiment(**setting)
        expobj.run()

if __name__ == '__main__':
    main()

