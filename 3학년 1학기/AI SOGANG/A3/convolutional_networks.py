"""
Implements convolutional networks in PyTorch.
WARNING: you SHOULD NOT use ".to()" or ".cuda()" in each implementation block.
"""
import torch
from a3_helper import softmax_loss
from fully_connected_networks import Linear_ReLU, Linear, Solver, adam, ReLU


def hello_convolutional_networks():
    """
    This is a sample function that we will try to import and run to ensure that
    our environment is correctly set up on Google Colab.
    """
    print('Hello from convolutional_networks.py!')


class Conv(object):

    @staticmethod
    def forward(x, w, b, conv_param):
        """
        A naive implementation of the forward pass for a convolutional layer.
        The input consists of N data points, each with C channels, height H and
        width W. We convolve each input with F different filters, where each
        filter spans all C channels and has height HH and width WW.

        Input:
        - x: Input data of shape (N, C, H, W)
        - w: Filter weights of shape (F, C, HH, WW)
        - b: Biases, of shape (F,)
        - conv_param: A dictionary with the following keys:
          - 'stride': The number of pixels between adjacent receptive fields
            in the horizontal and vertical directions.
          - 'pad': The number of pixels that is used to zero-pad the input.

        During padding, 'pad' zeros should be placed symmetrically (i.e equally
        on both sides) along the height and width axes of the input. Be careful
        not to modfiy the original input x directly.

        Returns a tuple of:
        - out: Output data of shape (N, F, H', W') where H' and W' are given by
          H' = 1 + (H + 2 * pad - HH) / stride
          W' = 1 + (W + 2 * pad - WW) / stride
        - cache: (x, w, b, conv_param)
        """
        N, C, H, W = x.shape
        F, _, HH, WW = w.shape
        pad = conv_param['pad']
        stride = conv_param['stride']
        
        H_prime = 1 + (H + 2 * pad - HH) // stride
        W_prime = 1 + (W + 2 * pad - WW) // stride
        
        x_pad = torch.nn.functional.pad(x, (pad, pad, pad, pad), mode='constant', value=0.0)
        out = torch.zeros(N, F, H_prime, W_prime, dtype=x.dtype, device=x.device)
        
        for n in range(N):
            for f in range(F):
                for i in range(H_prime):
                    for j in range(W_prime):
                        hs = i * stride
                        ws = j * stride
                        patch = x_pad[n, :, hs:hs+HH, ws:ws+WW]
                        out[n, f, i, j] = torch.sum(patch * w[f]) + b[f]
        #####################################################################
        #                          END OF YOUR CODE                         #
        #####################################################################
        cache = (x, w, b, conv_param)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        A naive implementation of the backward pass for a convolutional layer.
          Inputs:
        - dout: Upstream derivatives.
        - cache: A tuple of (x, w, b, conv_param) as in conv_forward_naive

        Returns a tuple of:
        - dx: Gradient with respect to x
        - dw: Gradient with respect to w
        - db: Gradient with respect to b
        """
        x, w, b, conv_param = cache
        pad = conv_param['pad']
        stride = conv_param['stride']
        N, C, H, W = x.shape
        F, _, HH, WW = w.shape
        H_prime = dout.shape[2]
        W_prime = dout.shape[3]
        
        x_pad = torch.nn.functional.pad(x, (pad, pad, pad, pad), mode='constant', value=0.0)
        dx_pad = torch.zeros_like(x_pad)
        dw = torch.zeros_like(w)
        db = torch.zeros_like(b)
        
        for n in range(N):
            for f in range(F):
                db[f] += dout[n, f].sum()
                for i in range(H_prime):
                    for j in range(W_prime):
                        hs = i * stride
                        ws = j * stride
                        dx_pad[n, :, hs:hs+HH, ws:ws+WW] += dout[n, f, i, j] * w[f]
                        dw[f] += dout[n, f, i, j] * x_pad[n, :, hs:hs+HH, ws:ws+WW]
                        
        if pad > 0:
            dx = dx_pad[:, :, pad:-pad, pad:-pad]
        else:
            dx = dx_pad
        ###############################################################
        #                       END OF YOUR CODE                      #
        ###############################################################
        return dx, dw, db


class MaxPool(object):

    @staticmethod
    def forward(x, pool_param):
        """
        A naive implementation of the forward pass for a max-pooling layer.

        Inputs:
        - x: Input data, of shape (N, C, H, W)
        - pool_param: dictionary with the following keys:
          - 'pool_height': The height of each pooling region
          - 'pool_width': The width of each pooling region
          - 'stride': The distance between adjacent pooling regions
        No padding is necessary here.

        Returns a tuple of:
        - out: Output of shape (N, C, H', W') where H' and W' are given by
          H' = 1 + (H - pool_height) / stride
          W' = 1 + (W - pool_width) / stride
        - cache: (x, pool_param)
        """
        N, C, H, W = x.shape
        ph = pool_param['pool_height']
        pw = pool_param['pool_width']
        stride = pool_param['stride']
        
        H_prime = 1 + (H - ph) // stride
        W_prime = 1 + (W - pw) // stride
        
        out = torch.zeros(N, C, H_prime, W_prime, dtype=x.dtype, device=x.device)
        
        for n in range(N):
            for c in range(C):
                for i in range(H_prime):
                    for j in range(W_prime):
                        hs = i * stride
                        ws = j * stride
                        patch = x[n, c, hs:hs+ph, ws:ws+pw]
                        out[n, c, i, j] = torch.max(patch)
        ####################################################################
        #                         END OF YOUR CODE                         #
        ####################################################################
        cache = (x, pool_param)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        A naive implementation of the backward pass for a max-pooling layer.
        Inputs:
        - dout: Upstream derivatives
        - cache: A tuple of (x, pool_param) as in the forward pass.
        Returns:
        - dx: Gradient with respect to x
        """
        x, pool_param = cache
        N, C, H, W = x.shape
        ph = pool_param['pool_height']
        pw = pool_param['pool_width']
        stride = pool_param['stride']
        
        H_prime = dout.shape[2]
        W_prime = dout.shape[3]
        
        dx = torch.zeros_like(x)
        
        for n in range(N):
            for c in range(C):
                for i in range(H_prime):
                    for j in range(W_prime):
                        hs = i * stride
                        ws = j * stride
                        patch = x[n, c, hs:hs+ph, ws:ws+pw]
                        flat_idx = torch.argmax(patch.flatten())
                        unravel_i = flat_idx // pw
                        unravel_j = flat_idx % pw
                        dx[n, c, hs + unravel_i, ws + unravel_j] += dout[n, c, i, j]
        ####################################################################
        #                          END OF YOUR CODE                        #
        ####################################################################
        return dx


class ThreeLayerConvNet(object):
    """
    A three-layer convolutional network with the following architecture:
    conv - relu - 2x2 max pool - linear - relu - linear - softmax
    The network operates on minibatches of data that have shape (N, C, H, W)
    consisting of N images, each with height H and width W and with C input
    channels.
    """

    def __init__(self,
                 input_dims=(3, 32, 32),
                 num_filters=32,
                 filter_size=7,
                 hidden_dim=100,
                 num_classes=10,
                 weight_scale=1e-3,
                 reg=0.0,
                 dtype=torch.float,
                 device='cpu'):
        """
        Initialize a new network.
        Inputs:
        - input_dims: Tuple (C, H, W) giving size of input data
        - num_filters: Number of filters to use in the convolutional layer
        - filter_size: Width/height of filters to use in convolutional layer
        - hidden_dim: Number of units to use in fully-connected hidden layer
        - num_classes: Number of scores to produce from the final linear layer.
        - weight_scale: Scalar giving standard deviation for random
          initialization of weights.
        - reg: Scalar giving L2 regularization strength
        - dtype: A torch data type object; all computations will be performed
          using this datatype. float is faster but less accurate, so you
          should use double for numeric gradient checking.
        - device: device to use for computation. 'cpu' or 'cuda'
        """
        self.params = {}
        self.reg = reg
        self.dtype = dtype

        ######################################################################
        # TODO: Initialize weights，biases for the three-layer convolutional #
        # network. Weights should be initialized from a Gaussian             #
        # centered at 0.0 with standard deviation equal to weight_scale;     #
        # biases should be initialized to zero. All weights and biases       #
        # should be stored in thedictionary self.params. Store weights and   #
        # biases for the convolutional layer using the keys 'W1' and 'b1';   #
        # use keys 'W2' and 'b2' for the weights and biases of the hidden    #
        # linear layer, and key 'W3' and 'b3' for the weights and biases of  #
        # the output linear layer                                            #
        #                                                                    #
        # IMPORTANT: For this assignment, you can assume that the padding    #
        # and stride of the first convolutional layer are chosen so that     #
        # **the width and height of the input are preserved**. Take a        #
        # look at the start of the loss() function to see how that happens.  #
        ######################################################################
        C, H, W = input_dims
        self.params['W1'] = torch.randn(num_filters, C, filter_size, filter_size, dtype=dtype, device=device) * weight_scale
        self.params['b1'] = torch.zeros(num_filters, dtype=dtype, device=device)
        
        H_pool = H // 2
        W_pool = W // 2
        input_linear_dim = num_filters * H_pool * W_pool
        
        self.params['W2'] = torch.randn(input_linear_dim, hidden_dim, dtype=dtype, device=device) * weight_scale
        self.params['b2'] = torch.zeros(hidden_dim, dtype=dtype, device=device)
        
        self.params['W3'] = torch.randn(hidden_dim, num_classes, dtype=dtype, device=device) * weight_scale
        self.params['b3'] = torch.zeros(num_classes, dtype=dtype, device=device)
        ######################################################################
        #                            END OF YOUR CODE                        #
        ######################################################################################

    def save(self, path):
        checkpoint = {
          'reg': self.reg,
          'dtype': self.dtype,
          'params': self.params,
        }
        torch.save(checkpoint, path)
        print("Saved in {}".format(path))

    def load(self, path):
        checkpoint = torch.load(path, map_location='cpu')
        self.params = checkpoint['params']
        self.dtype = checkpoint['dtype']
        self.reg = checkpoint['reg']
        print("load checkpoint file: {}".format(path))

    def loss(self, X, y=None):
        """
        Evaluate loss and gradient for the three-layer convolutional network.
        Input / output: Same API as TwoLayerNet.
        """
        X = X.to(self.dtype)
        W1, b1 = self.params['W1'], self.params['b1']
        W2, b2 = self.params['W2'], self.params['b2']
        W3, b3 = self.params['W3'], self.params['b3']

        # pass conv_param to the forward pass for the convolutional layer
        # Padding and stride chosen to preserve the input spatial size
        filter_size = W1.shape[2]
        conv_param = {'stride': 1, 'pad': (filter_size - 1) // 2}

        # pass pool_param to the forward pass for the max-pooling layer
        pool_param = {'pool_height': 2, 'pool_width': 2, 'stride': 2}

        scores = None
        ######################################################################
        # TODO: Implement the forward pass for three-layer convolutional     #
        # net, computing the class scores for X and storing them in the      #
        # scores variable.                                                   #
        #                                                                    #
        # Remember you can use functions defined in your implementation      #
        # above                                                              #
        ######################################################################
        a1, conv_cache = Conv.forward(X, W1, b1, conv_param)
        a2, relu_cache1 = ReLU.forward(a1)
        a3, pool_cache = MaxPool.forward(a2, pool_param)
        
        a4, fc_relu_cache = Linear_ReLU.forward(a3, W2, b2)
        scores, fc_cache = Linear.forward(a4, W3, b3)
        
        self.conv_cache = conv_cache
        self.relu_cache1 = relu_cache1
        self.pool_cache = pool_cache
        self.fc_relu_cache = fc_relu_cache
        self.fc_cache = fc_cache
        ######################################################################
        #                             END OF YOUR CODE                       #
        ######################################################################

        if y is None:
            return scores

        loss, grads = 0.0, {}
        ####################################################################
        # TODO: Implement backward pass for three-layer convolutional net, #
        # storing the loss and gradients in the loss and grads variables.  #
        # Compute data loss using softmax, and make sure that grads[k]     #
        # holds the gradients for self.params[k]. Don't forget to add      #
        # L2 regularization!                                               #
        #                                                                  #
        # NOTE: To ensure that your implementation matches ours and you    #
        # pass the automated tests, make sure that your L2 regularization  #
        # does not include a factor of 0.5                                 #
        ####################################################################
        loss, dscores = softmax_loss(scores, y)
        loss += self.reg * (torch.sum(W1 ** 2) + torch.sum(W2 ** 2) + torch.sum(W3 ** 2))
        
        da4, dw3, db3 = Linear.backward(dscores, self.fc_cache)
        da3, dw2, db2 = Linear_ReLU.backward(da4, self.fc_relu_cache)
        da2 = MaxPool.backward(da3, self.pool_cache)
        da1 = ReLU.backward(da2, self.relu_cache1)
        dx, dw1, db1 = Conv.backward(da1, self.conv_cache)
        
        grads['W3'] = dw3 + 2.0 * self.reg * W3
        grads['b3'] = db3
        grads['W2'] = dw2 + 2.0 * self.reg * W2
        grads['b2'] = db2
        grads['W1'] = dw1 + 2.0 * self.reg * W1
        grads['b1'] = db1
        ###################################################################
        #                             END OF YOUR CODE                    #
        ###################################################################

        return loss, grads


class DeepConvNet(object):
    """
    A convolutional neural network with an arbitrary number of convolutional
    layers in VGG-Net style. All convolution layers will use kernel size 3 and
    padding 1 to preserve the feature map size, and all pooling layers will be
    max pooling layers with 2x2 receptive fields and a stride of 2 to halve the
    size of the feature map.

    The network will have the following architecture:

    {conv - [batchnorm?] - relu - [pool?]} x (L - 1) - linear

    Each {...} structure is a "macro layer" consisting of a convolution layer,
    an optional batch normalization layer, a ReLU nonlinearity, and an optional
    pooling layer. After L-1 such macro layers, a single fully-connected layer
    is used to predict the class scores.

    The network operates on minibatches of data that have shape (N, C, H, W)
    consisting of N images, each with height H and width W and with C input
    channels.
    """
    def __init__(self,
                 input_dims=(3, 32, 32),
                 num_filters=[8, 8, 8, 8, 8],
                 max_pools=[0, 1, 2, 3, 4],
                 batchnorm=False,
                 num_classes=10,
                 weight_scale=1e-3,
                 reg=0.0,
                 weight_initializer=None,
                 dtype=torch.float,
                 device='cpu'):
        """
        Initialize a new network.

        Inputs:
        - input_dims: Tuple (C, H, W) giving size of input data
        - num_filters: List of length (L - 1) giving the number of
          convolutional filters to use in each macro layer.
        - max_pools: List of integers giving the indices of the macro
          layers that should have max pooling (zero-indexed).
        - batchnorm: Whether to include batch normalization in each macro layer
        - num_classes: Number of scores to produce from the final linear layer.
        - weight_scale: Scalar giving standard deviation for random
          initialization of weights, or the string "kaiming" to use Kaiming
          initialization instead
        - reg: Scalar giving L2 regularization strength. L2 regularization
          should only be applied to convolutional and fully-connected weight
          matrices; it should not be applied to biases or to batchnorm scale
          and shifts.
        - dtype: A torch data type object; all computations will be performed
          using this datatype. float is faster but less accurate, so you should
          use double for numeric gradient checking.
        - device: device to use for computation. 'cpu' or 'cuda'
        """
        self.params = {}
        self.num_layers = len(num_filters)+1
        self.max_pools = max_pools
        self.batchnorm = batchnorm
        self.reg = reg
        self.dtype = dtype

        if device == 'cuda':
            device = 'cuda:0'

        C, H, W = input_dims
        C_in = C
        H_in = H
        W_in = W
        
        for idx in range(len(num_filters)):
            i = idx + 1
            C_out = num_filters[idx]
            
            # Conv layer weights and biases
            if weight_scale == 'kaiming':
                self.params[f'W{i}'] = kaiming_initializer(C_in, C_out, K=3, relu=True, device=device, dtype=dtype)
            else:
                self.params[f'W{i}'] = torch.randn(C_out, C_in, 3, 3, dtype=dtype, device=device) * weight_scale
            self.params[f'b{i}'] = torch.zeros(C_out, dtype=dtype, device=device)
            
            # BatchNorm scale and shift
            if self.batchnorm:
                self.params[f'gamma{i}'] = torch.ones(C_out, dtype=dtype, device=device)
                self.params[f'beta{i}'] = torch.zeros(C_out, dtype=dtype, device=device)
                
            C_in = C_out
            if idx in self.max_pools:
                H_in = H_in // 2
                W_in = W_in // 2
                
        # Final linear layer
        L = self.num_layers
        D_in = C_in * H_in * W_in
        if weight_scale == 'kaiming':
            self.params[f'W{L}'] = kaiming_initializer(D_in, num_classes, K=None, relu=False, device=device, dtype=dtype)
        else:
            self.params[f'W{L}'] = torch.randn(D_in, num_classes, dtype=dtype, device=device) * weight_scale
        self.params[f'b{L}'] = torch.zeros(num_classes, dtype=dtype, device=device)
        ################################################################
        #                      END OF YOUR CODE                        #
        ################################################################

        # With batch normalization we need to keep track of running
        # means and variances, so we need to pass a special bn_param
        # object to each batch normalization layer. You should pass
        # self.bn_params[0] to the forward pass of the first batch
        # normalization layer, self.bn_params[1] to the forward
        # pass of the second batch normalization layer, etc.
        self.bn_params = []
        if self.batchnorm:
            self.bn_params = [{'mode': 'train'}
                              for _ in range(len(num_filters))]

        # Check that we got the right number of parameters
        if not self.batchnorm:
            params_per_macro_layer = 2  # weight and bias
        else:
            params_per_macro_layer = 4  # weight, bias, scale, shift
        num_params = params_per_macro_layer * len(num_filters) + 2
        msg = 'self.params has the wrong number of ' \
              'elements. Got %d; expected %d'
        msg = msg % (len(self.params), num_params)
        assert len(self.params) == num_params, msg

        # Check that all parameters have the correct device and dtype:
        for k, param in self.params.items():
            msg = 'param "%s" has device %r; should be %r' \
                  % (k, param.device, device)
            assert param.device == torch.device(device), msg
            msg = 'param "%s" has dtype %r; should be %r' \
                  % (k, param.dtype, dtype)
            assert param.dtype == dtype, msg

    def save(self, path):
        checkpoint = {
          'reg': self.reg,
          'dtype': self.dtype,
          'params': self.params,
          'num_layers': self.num_layers,
          'max_pools': self.max_pools,
          'batchnorm': self.batchnorm,
          'bn_params': self.bn_params,
        }
        torch.save(checkpoint, path)
        print("Saved in {}".format(path))

    def load(self, path, dtype, device):
        checkpoint = torch.load(path, map_location='cpu')
        self.params = checkpoint['params']
        self.dtype = dtype
        self.reg = checkpoint['reg']
        self.num_layers = checkpoint['num_layers']
        self.max_pools = checkpoint['max_pools']
        self.batchnorm = checkpoint['batchnorm']
        self.bn_params = checkpoint['bn_params']

        for p in self.params:
            self.params[p] = \
                self.params[p].type(dtype).to(device)

        for i in range(len(self.bn_params)):
            for p in ["running_mean", "running_var"]:
                self.bn_params[i][p] = \
                    self.bn_params[i][p].type(dtype).to(device)

        print("load checkpoint file: {}".format(path))

    def loss(self, X, y=None):
        """
        Evaluate loss and gradient for the deep convolutional
        network.
        Input / output: Same API as ThreeLayerConvNet.
        """
        X = X.to(self.dtype)
        mode = 'test' if y is None else 'train'

        # Set train/test mode for batchnorm params since they
        # behave differently during training and testing.
        if self.batchnorm:
            for bn_param in self.bn_params:
                bn_param['mode'] = mode
        scores = None

        # pass conv_param to the forward pass for the
        # convolutional layer
        # Padding and stride chosen to preserve the input
        # spatial size
        filter_size = 3
        conv_param = {'stride': 1, 'pad': (filter_size - 1) // 2}

        # pass pool_param to the forward pass for the max-pooling layer
        pool_param = {'pool_height': 2, 'pool_width': 2, 'stride': 2}

        scores = None
        #########################################################
        # TODO: Implement the forward pass for the DeepConvNet, #
        # computing the class scores for X and storing them in  #
        # the scores variable.                                  #
        #                                                       #
        # You should use the fast versions of convolution and   #
        # max pooling layers, or the convolutional sandwich     #
        # layers, to simplify your implementation.              #
        #########################################################
        self.caches = {}
        out = X
        
        for idx in range(self.num_layers - 1):
            i = idx + 1
            w = self.params[f'W{i}']
            b = self.params[f'b{i}']
            has_pool = idx in self.max_pools
            
            if self.batchnorm:
                gamma = self.params[f'gamma{i}']
                beta = self.params[f'beta{i}']
                bn_param = self.bn_params[idx]
                if has_pool:
                    out, cache = Conv_BatchNorm_ReLU_Pool.forward(out, w, b, gamma, beta, conv_param, bn_param, pool_param)
                else:
                    out, cache = Conv_BatchNorm_ReLU.forward(out, w, b, gamma, beta, conv_param, bn_param)
            else:
                if has_pool:
                    out, cache = Conv_ReLU_Pool.forward(out, w, b, conv_param, pool_param)
                else:
                    out, cache = Conv_ReLU.forward(out, w, b, conv_param)
            
            self.caches[i] = cache
            
        # Final linear layer
        L = self.num_layers
        w = self.params[f'W{L}']
        b = self.params[f'b{L}']
        scores, cache = Linear.forward(out, w, b)
        self.caches[L] = cache
        #####################################################
        #                 END OF YOUR CODE                  #
        #####################################################

        if y is None:
            return scores

        loss, grads = 0, {}
        ###################################################################
        # TODO: Implement the backward pass for the DeepConvNet,          #
        # storing the loss and gradients in the loss and grads variables. #
        # Compute data loss using softmax, and make sure that grads[k]    #
        # holds the gradients for self.params[k]. Don't forget to add     #
        # L2 regularization!                                              #
        #                                                                 #
        # NOTE: To ensure that your implementation matches ours and you   #
        # pass the automated tests, make sure that your L2 regularization #
        # does not include a factor of 0.5                                #
        ###################################################################
        loss, dscores = softmax_loss(scores, y)
        
        reg_loss = 0.0
        for i in range(1, self.num_layers + 1):
            reg_loss += torch.sum(self.params[f'W{i}'] ** 2)
        loss += self.reg * reg_loss
        
        # Backward final linear layer
        L = self.num_layers
        dout, dw, db = Linear.backward(dscores, self.caches[L])
        grads[f'W{L}'] = dw + 2.0 * self.reg * self.params[f'W{L}']
        grads[f'b{L}'] = db
        
        # Backward macro layers
        for idx in range(self.num_layers - 2, -1, -1):
            i = idx + 1
            has_pool = idx in self.max_pools
            cache = self.caches[i]
            
            if self.batchnorm:
                if has_pool:
                    dout, dw, db, dgamma, dbeta = Conv_BatchNorm_ReLU_Pool.backward(dout, cache)
                else:
                    dout, dw, db, dgamma, dbeta = Conv_BatchNorm_ReLU.backward(dout, cache)
                grads[f'W{i}'] = dw + 2.0 * self.reg * self.params[f'W{i}']
                grads[f'b{i}'] = db
                grads[f'gamma{i}'] = dgamma
                grads[f'beta{i}'] = dbeta
            else:
                if has_pool:
                    dout, dw, db = Conv_ReLU_Pool.backward(dout, cache)
                else:
                    dout, dw, db = Conv_ReLU.backward(dout, cache)
                grads[f'W{i}'] = dw + 2.0 * self.reg * self.params[f'W{i}']
                grads[f'b{i}'] = db
        #############################################################
        #                       END OF YOUR CODE                    #
        #############################################################

        return loss, grads


def find_overfit_parameters():
    weight_scale = 0.1
    learning_rate = 0.02
    ###########################################################
    #                       END OF YOUR CODE                  #
    ###########################################################
    return weight_scale, learning_rate


def create_convolutional_solver_instance(data_dict, dtype, device):
    model = DeepConvNet(
        num_filters=[32, 32, 32],
        max_pools=[0, 1, 2],
        batchnorm=True,
        dtype=dtype,
        device=device,
        weight_scale='kaiming'
    )
    solver = Solver(
        model,
        data_dict,
        update_rule=adam,
        optim_config={
            'learning_rate': 2e-3,
        },
        lr_decay=0.95,
        num_epochs=5,
        batch_size=100,
        print_every=100
    )
    solver.train()
    #########################################################
    #                  END OF YOUR CODE                     #
    #########################################################
    return solver


def kaiming_initializer(Din, Dout, K=None, relu=True, device='cpu',
                        dtype=torch.float32):
    """
    Implement Kaiming initialization for linear and convolution layers.

    Inputs:
    - Din, Dout: Integers giving the number of input and output dimensions
      for this layer
    - K: If K is None, then initialize weights for a linear layer with
      Din input dimensions and Dout output dimensions. Otherwise if K is
      a nonnegative integer then initialize the weights for a convolution
      layer with Din input channels, Dout output channels, and a kernel size
      of KxK.
    - relu: If ReLU=True, then initialize weights with a gain of 2 to
      account for a ReLU nonlinearity (Kaiming initializaiton); otherwise
      initialize weights with a gain of 1 (Xavier initialization).
    - device, dtype: The device and datatype for the output tensor.

    Returns:
    - weight: A torch Tensor giving initialized weights for this layer.
      For a linear layer it should have shape (Din, Dout); for a
      convolution layer it should have shape (Dout, Din, K, K).
    """
    gain = 2. if relu else 1.
    weight = None
    if K is None:
        std = (gain / Din) ** 0.5
        weight = torch.randn(Din, Dout, dtype=dtype, device=device) * std
        ###################################################################
        #                            END OF YOUR CODE                     #
        ###################################################################
    else:
        ###################################################################
        # TODO: Implement Kaiming initialization for convolutional layer. #
        # The weight scale is sqrt(gain / fan_in),                        #
        # where gain is 2 if ReLU is followed by the layer, or 1 if not,  #
        # and fan_in = num_in_channels (= Din) * K * K                    #
        # The output should be a tensor in the designated size, dtype,    #
        # and device.                                                     #
        ###################################################################
        fan_in = Din * K * K
        std = (gain / fan_in) ** 0.5
        weight = torch.randn(Dout, Din, K, K, dtype=dtype, device=device) * std
        ###################################################################
        #                         END OF YOUR CODE                        #
        ###################################################################
    return weight


class BatchNorm(object):

    @staticmethod
    def forward(x, gamma, beta, bn_param):
        """
        Forward pass for batch normalization.

        During training the sample mean and (uncorrected) sample variance
        are computed from minibatch statistics and used to normalize the
        incoming data. During training we also keep an exponentially decaying
        running mean of the mean and variance of each feature, and these
        averages are used to normalize data at test-time.

        At each timestep we update the running averages for mean and
        variance using an exponential decay based on the momentum parameter:

        running_mean = momentum * running_mean + (1 - momentum) * sample_mean
        running_var = momentum * running_var + (1 - momentum) * sample_var

        Note that the batch normalization paper suggests a different
        test-time behavior: they compute sample mean and variance for
        each feature using a large number of training images rather than
        using a running average. For this implementation we have chosen to use
        running averages instead since they do not require an additional
        estimation step; the PyTorch implementation of batch normalization
        also uses running averages.

        Input:
        - x: Data of shape (N, D)
        - gamma: Scale parameter of shape (D,)
        - beta: Shift paremeter of shape (D,)
        - bn_param: Dictionary with the following keys:
          - mode: 'train' or 'test'; required
          - eps: Constant for numeric stability
          - momentum: Constant for running mean / variance.
          - running_mean: Array of shape (D,) giving running mean
            of features
          - running_var Array of shape (D,) giving running variance
            of features

        Returns a tuple of:
        - out: of shape (N, D)
        - cache: A tuple of values needed in the backward pass
        """
        mode = bn_param['mode']
        eps = bn_param.get('eps', 1e-5)
        momentum = bn_param.get('momentum', 0.9)

        N, D = x.shape
        running_mean = bn_param.get('running_mean',
                                    torch.zeros(D,
                                                dtype=x.dtype,
                                                device=x.device))
        running_var = bn_param.get('running_var',
                                   torch.zeros(D,
                                               dtype=x.dtype,
                                               device=x.device))

        out, cache = None, None
        if mode == 'train':
            # Compute sample mean and variance
            sample_mean = x.mean(dim=0)
            sample_var = x.var(dim=0, unbiased=False)
            
            # Normalize
            x_centered = x - sample_mean
            std = torch.sqrt(sample_var + eps)
            x_norm = x_centered / std
            out = gamma * x_norm + beta
            
            # Update running averages
            running_mean = momentum * running_mean + (1 - momentum) * sample_mean
            running_var = momentum * running_var + (1 - momentum) * sample_var
            
            cache = (x, x_norm, x_centered, std, gamma, sample_mean, sample_var, eps)
            ################################################################
            #                           END OF YOUR CODE                   #
            ################################################################
        elif mode == 'test':
            ################################################################
            x_norm = (x - running_mean) / torch.sqrt(running_var + eps)
            out = gamma * x_norm + beta
            cache = (x, x_norm, None, torch.sqrt(running_var + eps), gamma, running_mean, running_var, eps)
            ################################################################
            #                      END OF YOUR CODE                        #
            ################################################################
        else:
            raise ValueError('Invalid forward batchnorm mode "%s"' % mode)

        # Store the updated running means back into bn_param
        bn_param['running_mean'] = running_mean.detach()
        bn_param['running_var'] = running_var.detach()

        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        Backward pass for batch normalization.

        For this implementation, you should write out a
        computation graph for batch normalization on paper and
        propagate gradients backward through intermediate nodes.

        Inputs:
        - dout: Upstream derivatives, of shape (N, D)
        - cache: Variable of intermediates from batchnorm_forward.

        Returns a tuple of:
        - dx: Gradient with respect to inputs x, of shape (N, D)
        - dgamma: Gradient with respect to scale parameter gamma,
          of shape (D,)
        - dbeta: Gradient with respect to shift parameter beta,
          of shape (D,)
        """
        dx, dgamma, dbeta = None, None, None
        # cache contains variables from forward pass
        x, x_norm, x_centered, std, gamma, sample_mean, sample_var, eps = cache
        N, D = x.shape
        
        dbeta = dout.sum(dim=0)
        dgamma = torch.sum(dout * x_norm, dim=0)
        
        if x_centered is None:
            # test mode backward (rarely used, but for completeness)
            dx = dout * gamma / std
            return dx, dgamma, dbeta
            
        dx_norm = dout * gamma
        dvar = torch.sum(dx_norm * x_centered * -0.5 * (std ** -3), dim=0)
        dmean = torch.sum(dx_norm * -1.0 / std, dim=0) + dvar * torch.sum(-2.0 * x_centered, dim=0) / N
        dx = dx_norm / std + dvar * 2.0 * x_centered / N + dmean / N
        #################################################################
        #                      END OF YOUR CODE                         #
        #################################################################

        return dx, dgamma, dbeta

    @staticmethod
    def backward_alt(dout, cache):
        """
        Alternative backward pass for batch normalization.
        For this implementation you should work out the derivatives
        for the batch normalizaton backward pass on paper and simplify
        as much as possible. You should be able to derive a simple expression
        for the backward pass. See the jupyter notebook for more hints.

        Note: This implementation should expect to receive the same
        cache variable as batchnorm_backward, but might not use all of
        the values in the cache.

        Inputs / outputs: Same as batchnorm_backward
        """
        dx, dgamma, dbeta = None, None, None
        x, x_norm, x_centered, std, gamma, sample_mean, sample_var, eps = cache
        N, D = x.shape
        
        dbeta = dout.sum(dim=0)
        dgamma = torch.sum(dout * x_norm, dim=0)
        
        dx_norm = dout * gamma
        dx = (N * dx_norm - dx_norm.sum(dim=0) - x_norm * torch.sum(dx_norm * x_norm, dim=0)) / (N * std)
        #################################################################
        #                        END OF YOUR CODE                       #
        #################################################################

        return dx, dgamma, dbeta


class SpatialBatchNorm(object):

    @staticmethod
    def forward(x, gamma, beta, bn_param):
        """
        Computes the forward pass for spatial batch normalization.

        Inputs:
        - x: Input data of shape (N, C, H, W)
        - gamma: Scale parameter, of shape (C,)
        - beta: Shift parameter, of shape (C,)
        - bn_param: Dictionary with the following keys:
          - mode: 'train' or 'test'; required
          - eps: Constant for numeric stability
          - momentum: Constant for running mean / variance. momentum=0
            means that old information is discarded completely at every
            time step, while momentum=1 means that new information is never
            incorporated. The default of momentum=0.9 should work well
            in most situations.
          - running_mean: Array of shape (C,) giving running mean of
            features
          - running_var Array of shape (C,) giving running variance
            of features

        Returns a tuple of:
        - out: Output data, of shape (N, C, H, W)
        - cache: Values needed for the backward pass
        """
        out, cache = None, None

        N, C, H, W = x.shape
        x_flat = x.permute(0, 2, 3, 1).reshape(-1, C)
        out_flat, cache = BatchNorm.forward(x_flat, gamma, beta, bn_param)
        out = out_flat.reshape(N, H, W, C).permute(0, 3, 1, 2)
        ################################################################
        #                       END OF YOUR CODE                       #
        ################################################################

        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        Computes the backward pass for spatial batch normalization.
        Inputs:
        - dout: Upstream derivatives, of shape (N, C, H, W)
        - cache: Values from the forward pass
        Returns a tuple of:
        - dx: Gradient with respect to inputs, of shape (N, C, H, W)
        - dgamma: Gradient with respect to scale parameter, of shape (C,)
        - dbeta: Gradient with respect to shift parameter, of shape (C,)
        """
        dx, dgamma, dbeta = None, None, None

        N, C, H, W = dout.shape
        dout_flat = dout.permute(0, 2, 3, 1).reshape(-1, C)
        dx_flat, dgamma, dbeta = BatchNorm.backward(dout_flat, cache)
        dx = dx_flat.reshape(N, H, W, C).permute(0, 3, 1, 2)
        ##################################################################
        #                       END OF YOUR CODE                         #
        ##################################################################

        return dx, dgamma, dbeta

##################################################################
#           Fast Implementations and Sandwich Layers             #
##################################################################


class FastConv(object):

    @staticmethod
    def forward(x, w, b, conv_param):
        N, C, H, W = x.shape
        F, _, HH, WW = w.shape
        stride, pad = conv_param['stride'], conv_param['pad']
        layer = torch.nn.Conv2d(C, F, (HH, WW), stride=stride, padding=pad)
        layer.weight = torch.nn.Parameter(w)
        layer.bias = torch.nn.Parameter(b)
        tx = x.detach()
        tx.requires_grad = True
        out = layer(tx)
        cache = (x, w, b, conv_param, tx, out, layer)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        try:
            x, _, _, _, tx, out, layer = cache
            out.backward(dout)
            dx = tx.grad.detach()
            dw = layer.weight.grad.detach()
            db = layer.bias.grad.detach()
            layer.weight.grad = layer.bias.grad = None
        except RuntimeError:
            dx, dw, db = torch.zeros_like(tx), \
                         torch.zeros_like(layer.weight), \
                         torch.zeros_like(layer.bias)
        return dx, dw, db


class FastMaxPool(object):

    @staticmethod
    def forward(x, pool_param):
        N, C, H, W = x.shape
        pool_height, pool_width = \
            pool_param['pool_height'], pool_param['pool_width']
        stride = pool_param['stride']
        layer = torch.nn.MaxPool2d(kernel_size=(pool_height, pool_width),
                                   stride=stride)
        tx = x.detach()
        tx.requires_grad = True
        out = layer(tx)
        cache = (x, pool_param, tx, out, layer)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        try:
            x, _, tx, out, layer = cache
            out.backward(dout)
            dx = tx.grad.detach()
        except RuntimeError:
            dx = torch.zeros_like(tx)
        return dx


class Conv_ReLU(object):

    @staticmethod
    def forward(x, w, b, conv_param):
        """
        A convenience layer that performs a convolution
        followed by a ReLU.
        Inputs:
        - x: Input to the convolutional layer
        - w, b, conv_param: Weights and parameters for the
          convolutional layer
        Returns a tuple of:
        - out: Output from the ReLU
        - cache: Object to give to the backward pass
        """
        a, conv_cache = FastConv.forward(x, w, b, conv_param)
        out, relu_cache = ReLU.forward(a)
        cache = (conv_cache, relu_cache)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        Backward pass for the conv-relu convenience layer.
        """
        conv_cache, relu_cache = cache
        da = ReLU.backward(dout, relu_cache)
        dx, dw, db = FastConv.backward(da, conv_cache)
        return dx, dw, db


class Conv_ReLU_Pool(object):

    @staticmethod
    def forward(x, w, b, conv_param, pool_param):
        """
        A convenience layer that performs a convolution,
        a ReLU, and a pool.
        Inputs:
        - x: Input to the convolutional layer
        - w, b, conv_param: Weights and parameters for
          the convolutional layer
        - pool_param: Parameters for the pooling layer
        Returns a tuple of:
        - out: Output from the pooling layer
        - cache: Object to give to the backward pass
        """
        a, conv_cache = FastConv.forward(x, w, b, conv_param)
        s, relu_cache = ReLU.forward(a)
        out, pool_cache = FastMaxPool.forward(s, pool_param)
        cache = (conv_cache, relu_cache, pool_cache)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        Backward pass for the conv-relu-pool
        convenience layer
        """
        conv_cache, relu_cache, pool_cache = cache
        ds = FastMaxPool.backward(dout, pool_cache)
        da = ReLU.backward(ds, relu_cache)
        dx, dw, db = FastConv.backward(da, conv_cache)
        return dx, dw, db


class Linear_BatchNorm_ReLU(object):

    @staticmethod
    def forward(x, w, b, gamma, beta, bn_param):
        """
        Convenience layer that performs an linear transform,
        batch normalization, and ReLU.
        Inputs:
        - x: Array of shape (N, D1); input to the linear layer
        - w, b: Arrays of shape (D2, D2) and (D2,) giving the
          weight and bias for the linear transform.
        - gamma, beta: Arrays of shape (D2,) and (D2,) giving
          scale and shift parameters for batch normalization.
        - bn_param: Dictionary of parameters for batch
          normalization.
        Returns:
        - out: Output from ReLU, of shape (N, D2)
        - cache: Object to give to the backward pass.
        """
        a, fc_cache = Linear.forward(x, w, b)
        a_bn, bn_cache = BatchNorm.forward(a, gamma, beta, bn_param)
        out, relu_cache = ReLU.forward(a_bn)
        cache = (fc_cache, bn_cache, relu_cache)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        """
        Backward pass for the linear-batchnorm-relu
        convenience layer.
        """
        fc_cache, bn_cache, relu_cache = cache
        da_bn = ReLU.backward(dout, relu_cache)
        da, dgamma, dbeta = BatchNorm.backward(da_bn, bn_cache)
        dx, dw, db = Linear.backward(da, fc_cache)
        return dx, dw, db, dgamma, dbeta


class Conv_BatchNorm_ReLU(object):

    @staticmethod
    def forward(x, w, b, gamma, beta, conv_param, bn_param):
        a, conv_cache = FastConv.forward(x, w, b, conv_param)
        an, bn_cache = SpatialBatchNorm.forward(a, gamma,
                                                beta, bn_param)
        out, relu_cache = ReLU.forward(an)
        cache = (conv_cache, bn_cache, relu_cache)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        conv_cache, bn_cache, relu_cache = cache
        dan = ReLU.backward(dout, relu_cache)
        da, dgamma, dbeta = SpatialBatchNorm.backward(dan, bn_cache)
        dx, dw, db = FastConv.backward(da, conv_cache)
        return dx, dw, db, dgamma, dbeta


class Conv_BatchNorm_ReLU_Pool(object):

    @staticmethod
    def forward(x, w, b, gamma, beta, conv_param, bn_param, pool_param):
        a, conv_cache = FastConv.forward(x, w, b, conv_param)
        an, bn_cache = SpatialBatchNorm.forward(a, gamma, beta, bn_param)
        s, relu_cache = ReLU.forward(an)
        out, pool_cache = FastMaxPool.forward(s, pool_param)
        cache = (conv_cache, bn_cache, relu_cache, pool_cache)
        return out, cache

    @staticmethod
    def backward(dout, cache):
        conv_cache, bn_cache, relu_cache, pool_cache = cache
        ds = FastMaxPool.backward(dout, pool_cache)
        dan = ReLU.backward(ds, relu_cache)
        da, dgamma, dbeta = SpatialBatchNorm.backward(dan, bn_cache)
        dx, dw, db = FastConv.backward(da, conv_cache)
        return dx, dw, db, dgamma, dbeta
